/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2007,2008,2009,2010,2011  Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.

 * Authors:
 *  Matthew Allum
 *  Robert Bragg
 *  Neil Roberts
 *  Emmanuele Bassi
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CLUTTER_ENABLE_EXPERIMENTAL_API

#include "clutter-config.h"

#include <stdlib.h>

#include "clutter-stage-cogl.h"

#include "clutter-actor-private.h"
#include "clutter-backend-private.h"
#include "clutter-debug.h"
#include "clutter-event.h"
#include "clutter-enum-types.h"
#include "clutter-feature.h"
#include "clutter-main.h"
#include "clutter-private.h"
#include "clutter-stage-private.h"

static void clutter_stage_window_iface_init (ClutterStageWindowIface *iface);

G_DEFINE_TYPE_WITH_CODE (ClutterStageCogl,
                         _clutter_stage_cogl,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_STAGE_WINDOW,
                                                clutter_stage_window_iface_init));

enum {
  PROP_0,
  PROP_WRAPPER,
  PROP_BACKEND,
  PROP_LAST
};

static void
clutter_stage_cogl_unrealize (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  int i;

  CLUTTER_NOTE (BACKEND, "Unrealizing Cogl stage [%p]", stage_cogl);

  if (stage_cogl->frame_closure != NULL)
    {
      _clutter_stage_window_remove_frame_callback (stage_window,
                                                   stage_cogl->frame_closure);
      stage_cogl->frame_closure = NULL;
    }

  for (i = 0; i < stage_cogl->num_outputs; i++)
    {
      ClutterStageOutput *stage_output = &stage_cogl->outputs[i].base;

      cogl_object_unref (stage_output->framebuffer);
    }

  g_free (stage_cogl->outputs);
}

static void
frame_cb (CoglOnscreen  *onscreen,
          CoglFrameEvent event,
          CoglFrameInfo *info,
          void          *user_data)
{
  ClutterStageCogl *stage_cogl = user_data;

  if (event == COGL_FRAME_EVENT_SYNC)
    {
      /* Early versions of the swap_event implementation in Mesa
       * deliver BufferSwapComplete event when not selected for,
       * so if we get a swap event we aren't expecting, just ignore it.
       *
       * https://bugs.freedesktop.org/show_bug.cgi?id=27962
       *
       * FIXME: This issue can be hidden inside Cogl so we shouldn't
       * need to care about this bug here.
       */
      if (stage_cogl->pending_swaps > 0)
        stage_cogl->pending_swaps--;
    }
  else if (event == COGL_FRAME_EVENT_COMPLETE)
    {
      gint64 presentation_time_cogl = cogl_frame_info_get_presentation_time (info);

      if (presentation_time_cogl != 0)
        {
          CoglContext *context = cogl_framebuffer_get_context (COGL_FRAMEBUFFER (onscreen));
          gint64 current_time_cogl = cogl_get_clock_time (context);
          gint64 now = g_get_monotonic_time ();

          stage_cogl->last_presentation_time =
            now + (presentation_time_cogl - current_time_cogl) / 1000;
        }

      stage_cogl->refresh_rate = cogl_frame_info_get_refresh_rate (info);
    }
}

static gboolean
clutter_stage_cogl_realize (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  ClutterBackend *backend;

  CLUTTER_NOTE (BACKEND, "Realizing stage '%s' [%p]",
                G_OBJECT_TYPE_NAME (stage_cogl),
                stage_cogl);

  backend = clutter_get_default_backend ();

  if (backend->cogl_context == NULL)
    {
      g_warning ("Failed to realize stage: missing Cogl context");
      return FALSE;
    }

  return TRUE;
}

static void
clutter_stage_cogl_schedule_update (ClutterStageWindow *stage_window,
                                    gint                sync_delay)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  gint64 now;
  float refresh_rate;
  gint64 refresh_interval;

  if (stage_cogl->update_time != -1)
    return;

  now = g_get_monotonic_time ();

  if (sync_delay < 0)
    {
      stage_cogl->update_time = now;
      return;
    }

  /* We only extrapolate presentation times for 150ms  - this is somewhat
   * arbitrary. The reasons it might not be accurate for larger times are
   * that the refresh interval might be wrong or the vertical refresh
   * might be downclocked if nothing is going on onscreen.
   */
  if (stage_cogl->last_presentation_time == 0||
      stage_cogl->last_presentation_time < now - 150000)
    {
      stage_cogl->update_time = now;
      return;
    }

  refresh_rate = stage_cogl->refresh_rate;
  if (refresh_rate == 0.0)
    refresh_rate = 60.0;

  refresh_interval = (gint64) (0.5 + 1000000 / refresh_rate);
  if (refresh_interval == 0)
    refresh_interval = 16667; /* 1/60th second */

  stage_cogl->update_time = stage_cogl->last_presentation_time + 1000 * sync_delay;

  while (stage_cogl->update_time < now)
    stage_cogl->update_time += refresh_interval;
}

static gint64
clutter_stage_cogl_get_update_time (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  if (stage_cogl->pending_swaps)
    return -1; /* in the future, indefinite */

  return stage_cogl->update_time;
}

static void
clutter_stage_cogl_clear_update_time (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  stage_cogl->update_time = -1;
}

static ClutterActor *
clutter_stage_cogl_get_wrapper (ClutterStageWindow *stage_window)
{
  return CLUTTER_ACTOR (CLUTTER_STAGE_COGL (stage_window)->wrapper);
}

static void
clutter_stage_cogl_show (ClutterStageWindow *stage_window,
			 gboolean            do_raise)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  clutter_actor_map (CLUTTER_ACTOR (stage_cogl->wrapper));
}

static void
clutter_stage_cogl_hide (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  clutter_actor_unmap (CLUTTER_ACTOR (stage_cogl->wrapper));
}

static void
clutter_stage_cogl_get_geometry (ClutterStageWindow    *stage_window,
                                 cairo_rectangle_int_t *geometry)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  if (geometry != NULL)
    {
      if (stage_cogl->outputs)
        {
          int i;
          cairo_region_t *region;
          cairo_rectangle_int_t extents;

          region = cairo_region_create ();
          for (i = 0; i < stage_cogl->num_outputs; i++)
            {
              ClutterStageOutput *stage_output = &stage_cogl->outputs[i].base;
              cairo_region_union_rectangle (region,
                                            &(cairo_rectangle_int_t) {
                                              .x = stage_output->x,
                                              .y = stage_output->y,
                                              .width = stage_output->width,
                                              .height = stage_output->height,
                                            });
            }

          cairo_region_get_extents (region, &extents);
          cairo_region_destroy (region);

          geometry->x = geometry->y = 0;
          geometry->width = extents.width;
          geometry->height = extents.height;
        }
      else
        {
          geometry->x = geometry->y = 0;
          geometry->width = 800;
          geometry->height = 600;
        }
    }
}

static void
clutter_stage_cogl_resize (ClutterStageWindow *stage_window,
                           gint                width,
                           gint                height)
{
}

static gboolean
clutter_stage_cogl_has_redraw_clips (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* NB: at the start of each new frame there is an implied clip that
   * clips everything (i.e. nothing would be drawn) so we need to make
   * sure we return True in the un-initialized case here.
   *
   * NB: a clip width of 0 means a full stage redraw has been queued
   * so we effectively don't have any redraw clips in that case.
   */
  if (!stage_cogl->initialized_redraw_clip ||
      (stage_cogl->initialized_redraw_clip &&
       stage_cogl->bounding_redraw_clip.width != 0))
    return TRUE;
  else
    return FALSE;
}

static gboolean
clutter_stage_cogl_ignoring_redraw_clips (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* NB: a clip width of 0 means a full stage redraw is required */
  if (stage_cogl->initialized_redraw_clip &&
      stage_cogl->bounding_redraw_clip.width == 0)
    return TRUE;
  else
    return FALSE;
}

/* A redraw clip represents (in stage coordinates) the bounding box of
 * something that needs to be redraw. Typically they are added to the
 * StageWindow as a result of clutter_actor_queue_clipped_redraw() by
 * actors such as ClutterGLXTexturePixmap. All redraw clips are
 * discarded after the next paint.
 *
 * A NULL stage_clip means the whole stage needs to be redrawn.
 *
 * What we do with this information:
 * - we keep track of the bounding box for all redraw clips
 * - when we come to redraw; we scissor the redraw to that box and use
 *   glBlitFramebuffer to present the redraw to the front
 *   buffer.
 */
static void
clutter_stage_cogl_add_redraw_clip (ClutterStageWindow    *stage_window,
                                    cairo_rectangle_int_t *stage_clip)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* If we are already forced to do a full stage redraw then bail early */
  if (clutter_stage_cogl_ignoring_redraw_clips (stage_window))
    return;

  /* A NULL stage clip means a full stage redraw has been queued and
   * we keep track of this by setting a zero width
   * stage_cogl->bounding_redraw_clip */
  if (stage_clip == NULL)
    {
      stage_cogl->bounding_redraw_clip.width = 0;
      stage_cogl->initialized_redraw_clip = TRUE;
      return;
    }

  /* Ignore requests to add degenerate/empty clip rectangles */
  if (stage_clip->width == 0 || stage_clip->height == 0)
    return;

  if (!stage_cogl->initialized_redraw_clip)
    {
      stage_cogl->bounding_redraw_clip = *stage_clip;
    }
  else if (stage_cogl->bounding_redraw_clip.width > 0)
    {
      _clutter_util_rectangle_union (&stage_cogl->bounding_redraw_clip,
                                     stage_clip,
                                     &stage_cogl->bounding_redraw_clip);
    }

  stage_cogl->initialized_redraw_clip = TRUE;
}

static gboolean
clutter_stage_cogl_get_redraw_clip_bounds (ClutterStageWindow    *stage_window,
                                           cairo_rectangle_int_t *stage_clip)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  if (stage_cogl->using_clipped_redraw)
    {
      *stage_clip = stage_cogl->bounding_redraw_clip;

      return TRUE;
    }

  return FALSE;
}

static inline gboolean
valid_buffer_age (ClutterStageCogl *stage_cogl, int age)
{
  if (age <= 0 || stage_cogl->dirty_backbuffer)
    return FALSE;

  return age < MIN (stage_cogl->damage_index, DAMAGE_HISTORY_MAX);
}

static gboolean
swap_framebuffer (ClutterStageWindow *stage_window,
                  ClutterStageCoglOutput *cogl_output,
                  cairo_rectangle_int_t *swap_region,
                  gboolean force_swap)
{
  CoglFramebuffer *framebuffer = cogl_output->base.framebuffer;
  int damage[4], ndamage;

  damage[0] = swap_region->x;
  damage[1] = swap_region->y;
  damage[2] = swap_region->width;
  damage[3] = swap_region->height;

  if (swap_region->width != 0)
    ndamage = 1;
  else
    ndamage = -1;

  if (cogl_is_onscreen (framebuffer))
    {
      CoglOnscreen *onscreen = COGL_ONSCREEN (framebuffer);

      /* push on the screen */
      if (ndamage == 1 && !force_swap)
        {
          CLUTTER_NOTE (BACKEND,
                        "cogl_onscreen_swap_region (onscreen: %p, "
                        "x: %d, y: %d, "
                        "width: %d, height: %d)",
                        onscreen,
                        damage[0], damage[1], damage[2], damage[3]);

          cogl_onscreen_swap_region (onscreen,
                                     damage, ndamage);

          return FALSE;
        }
      else
        {
          CLUTTER_NOTE (BACKEND, "cogl_onscreen_swap_buffers (onscreen: %p)",
                        onscreen);

          cogl_onscreen_swap_buffers_with_damage (onscreen,
                                                  damage, ndamage);

          return TRUE;
        }
    }
  else
    {
      CLUTTER_NOTE (BACKEND, "cogl_framebuffer_finish (framebuffer: %p)",
                    framebuffer);
      cogl_framebuffer_finish (framebuffer);

      return TRUE;
    }
}

static void
paint_stage (ClutterStageCogl *stage_cogl,
             ClutterStageOutput *stage_output,
             const cairo_rectangle_int_t *clip)
{
  ClutterStage *stage = stage_cogl->wrapper;

  /* FIXME: Need per framebuffer dirty bits, but for now just always
   * reset. */
  _clutter_stage_dirty_viewport (stage);
  _clutter_stage_dirty_projection (stage);
  _clutter_stage_maybe_setup_viewport (stage, stage_output);

  _clutter_stage_do_paint (stage, clip);
}

/* XXX: This is basically identical to clutter_stage_glx_redraw */
static gboolean
clutter_stage_cogl_redraw_output (ClutterStageWindow *stage_window,
                                  ClutterStageCoglOutput *cogl_output)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  ClutterStageOutput *stage_output = &cogl_output->base;
  CoglFramebuffer *fb = cogl_output->base.framebuffer;
  cairo_rectangle_int_t geom;
  gboolean have_clip;
  gboolean may_use_clipped_redraw;
  gboolean use_clipped_redraw;
  gboolean can_blit_sub_buffer;
  gboolean has_buffer_age;
  gboolean do_swap_buffer;
  gboolean force_swap;
  ClutterActor *wrapper;
  cairo_rectangle_int_t output_rect;
  cairo_rectangle_int_t redraw_clip;
  cairo_rectangle_int_t swap_region;
  cairo_rectangle_int_t clip_region;
  gboolean clip_region_empty;
  int window_scale;
  int fb_scale;

  wrapper = CLUTTER_ACTOR (stage_cogl->wrapper);

  can_blit_sub_buffer =
    cogl_is_onscreen (fb) &&
    cogl_clutter_winsys_has_feature (COGL_WINSYS_FEATURE_SWAP_REGION);

  has_buffer_age =
    cogl_is_onscreen (fb) &&
    cogl_clutter_winsys_has_feature (COGL_WINSYS_FEATURE_BUFFER_AGE);

  _clutter_stage_window_get_geometry (stage_window, &geom);

  output_rect = (cairo_rectangle_int_t) {
    .x = stage_output->x,
    .y = stage_output->y,
    .width = stage_output->width,
    .height = stage_output->height,
  };

  /* NB: a zero width redraw clip == full stage redraw */
  if (stage_cogl->bounding_redraw_clip.width == 0)
    have_clip = FALSE;
  else
    {
      redraw_clip = stage_cogl->bounding_redraw_clip;
      _clutter_util_rectangle_intersection (&redraw_clip, &output_rect,
                                            &redraw_clip);

      have_clip = !(redraw_clip.x == output_rect.x &&
                    redraw_clip.y == output_rect.y &&
                    redraw_clip.width == output_rect.width &&
                    redraw_clip.height == output_rect.height);
    }

  may_use_clipped_redraw = FALSE;
  if (_clutter_stage_window_can_clip_redraws (stage_window) &&
      (can_blit_sub_buffer || has_buffer_age) &&
      have_clip &&
      /* some drivers struggle to get going and produce some junk
       * frames when starting up... */
      stage_cogl->frame_count > 3)
    {
      may_use_clipped_redraw = TRUE;
      clip_region = redraw_clip;
    }
  else
    clip_region = (cairo_rectangle_int_t){ 0 };

  if (may_use_clipped_redraw &&
      G_LIKELY (!(clutter_paint_debug_flags & CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS)))
    use_clipped_redraw = TRUE;
  else
    use_clipped_redraw = FALSE;

  force_swap = FALSE;

  window_scale = _clutter_stage_window_get_scale_factor (stage_window);
  fb_scale = window_scale * stage_output->scale;

  if (has_buffer_age)
    {
      cairo_rectangle_int_t *current_damage =
	&stage_cogl->damage_history[DAMAGE_HISTORY (stage_cogl->damage_index++)];

      if (use_clipped_redraw)
	{
	  int age, i;

          age = cogl_onscreen_get_buffer_age (COGL_ONSCREEN (fb));

	  *current_damage = clip_region;

	  if (valid_buffer_age (stage_cogl, age))
	    {
              /* FIXME: This is currently broken, damage history needs to be
               * per output. */
	      for (i = 1; i <= age; i++)
		_clutter_util_rectangle_union (&clip_region,
					       &stage_cogl->damage_history[DAMAGE_HISTORY (stage_cogl->damage_index - i - 1)],
					       &clip_region);

              /* Update the bounding redraw clip state with the extra damage. */
              _clutter_util_rectangle_union (&stage_cogl->bounding_redraw_clip,
                                             &clip_region,
                                             &stage_cogl->bounding_redraw_clip);

	      CLUTTER_NOTE (CLIPPING, "Reusing back buffer(age=%d) - repairing region: x=%d, y=%d, width=%d, height=%d\n",
			    age,
			    clip_region.x,
			    clip_region.y,
			    clip_region.width,
			    clip_region.height);
	      force_swap = TRUE;
	    }
	  else
	    {
	      CLUTTER_NOTE (CLIPPING, "Invalid back buffer(age=%d): forcing full redraw\n", age);
	      use_clipped_redraw = FALSE;
	    }
	}
      else
	{
	  current_damage->x = 0;
	  current_damage->y = 0;
	  current_damage->width  = geom.width;
	  current_damage->height = geom.height;
	}
    }

  clip_region_empty = use_clipped_redraw && clip_region.width == 0;

  cogl_push_framebuffer (fb);
  if (use_clipped_redraw && clip_region_empty)
    {
      CLUTTER_NOTE (CLIPPING, "Empty stage output paint\n");
    }
  else if (use_clipped_redraw)
    {
      int scissor_x;
      int scissor_y;

      CLUTTER_NOTE (CLIPPING,
                    "Stage clip pushed: x=%d, y=%d, width=%d, height=%d\n",
                    clip_region.x,
                    clip_region.y,
                    clip_region.width,
                    clip_region.height);

      stage_cogl->using_clipped_redraw = TRUE;

      scissor_x = (clip_region.x - stage_output->x) * fb_scale;
      scissor_y = (clip_region.y - stage_output->y) * fb_scale;
      cogl_framebuffer_push_scissor_clip (fb,
                                          scissor_x,
                                          scissor_y,
                                          clip_region.width * fb_scale,
                                          clip_region.height * fb_scale);
      paint_stage (stage_cogl, stage_output, &clip_region);
      cogl_framebuffer_pop_clip (fb);

      stage_cogl->using_clipped_redraw = FALSE;
    }
  else
    {
      CLUTTER_NOTE (CLIPPING, "Unclipped stage paint\n");

      /* If we are trying to debug redraw issues then we want to pass
       * the bounding_redraw_clip so it can be visualized */
      if (G_UNLIKELY (clutter_paint_debug_flags & CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS) &&
          may_use_clipped_redraw &&
          !clip_region_empty)
        {
          int scissor_x;
          int scissor_y;

          scissor_x = (clip_region.x - stage_output->x) * fb_scale;
          scissor_y = (clip_region.y - stage_output->y) * fb_scale;
          cogl_framebuffer_push_scissor_clip (fb,
                                              scissor_x,
                                              scissor_y,
                                              clip_region.width * fb_scale,
                                              clip_region.height * fb_scale);
          paint_stage (stage_cogl, stage_output, &clip_region);
          cogl_framebuffer_pop_clip (fb);
        }
      else
        paint_stage (stage_cogl, stage_output, &output_rect);
    }
  cogl_pop_framebuffer ();

  if (may_use_clipped_redraw &&
      G_UNLIKELY ((clutter_paint_debug_flags & CLUTTER_DEBUG_REDRAWS)))
    {
      CoglContext *ctx = cogl_framebuffer_get_context (fb);
      static CoglPipeline *outline = NULL;
      ClutterActor *actor = CLUTTER_ACTOR (wrapper);
      float x_1 = redraw_clip.x;
      float x_2 = redraw_clip.x + redraw_clip.width;
      float y_1 = redraw_clip.y;
      float y_2 = redraw_clip.y + redraw_clip.height;
      CoglVertexP2 quad[4] = {
        { x_1, y_1 },
        { x_2, y_1 },
        { x_2, y_2 },
        { x_1, y_2 }
      };
      CoglPrimitive *prim;
      CoglMatrix modelview;

      if (outline == NULL)
        {
          outline = cogl_pipeline_new (ctx);
          cogl_pipeline_set_color4ub (outline, 0xff, 0x00, 0x00, 0xff);
        }

      prim = cogl_primitive_new_p2 (ctx,
                                    COGL_VERTICES_MODE_LINE_LOOP,
                                    4, /* n_vertices */
                                    quad);

      cogl_framebuffer_push_matrix (fb);
      cogl_matrix_init_identity (&modelview);
      _clutter_actor_apply_modelview_transform (actor, &modelview);
      cogl_framebuffer_set_modelview_matrix (fb, &modelview);
      cogl_framebuffer_draw_primitive (fb, outline, prim);
      cogl_framebuffer_pop_matrix (fb);
      cogl_object_unref (prim);
    }

  /* XXX: It seems there will be a race here in that the stage
   * window may be resized before the cogl_onscreen_swap_region
   * is handled and so we may copy the wrong region. I can't
   * really see how we can handle this with the current state of X
   * but at least in this case a full redraw should be queued by
   * the resize anyway so it should only exhibit temporary
   * artefacts.
   */
  if (use_clipped_redraw || force_swap)
    {
      if (use_clipped_redraw && clip_region_empty && !force_swap)
        do_swap_buffer = FALSE;
      else if (use_clipped_redraw)
        {
          swap_region = (cairo_rectangle_int_t) {
            .x = (clip_region.x - stage_output->x) * fb_scale,
            .y = (clip_region.y - stage_output->y) * fb_scale,
            .width = clip_region.width * fb_scale,
            .height = clip_region.height * fb_scale,
          };
          g_assert (swap_region.width > 0);
          do_swap_buffer = TRUE;
        }
      else
        {
          swap_region = (cairo_rectangle_int_t) {
            .x = 0,
            .y = 0,
            .width = output_rect.width * fb_scale,
            .height = output_rect.height * fb_scale,
          };
          do_swap_buffer = TRUE;
        }
    }
  else
    {
      swap_region = output_rect;
      do_swap_buffer = TRUE;
    }

  if (do_swap_buffer)
    {
      return swap_framebuffer (stage_window,
                               cogl_output,
                               &swap_region,
                               force_swap);
    }
  else
    return FALSE;
}

static void
clutter_stage_cogl_redraw (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  gboolean swap_event;
  int i;

  g_assert (stage_cogl->num_outputs > 0);

  for (i = 0; i < stage_cogl->num_outputs; i++)
    {
      ClutterStageCoglOutput *stage_output = &stage_cogl->outputs[i];

      swap_event =
        clutter_stage_cogl_redraw_output (stage_window,
                                          stage_output) ||
        swap_event;
    }

  _clutter_stage_window_swap_buffers (stage_window);

  if (swap_event)
    {
      /* If we have swap buffer events then cogl_onscreen_swap_buffers
       * will return immediately and we need to track that there is a
       * swap in progress... */
      if (clutter_feature_available (CLUTTER_FEATURE_SWAP_EVENTS))
        stage_cogl->pending_swaps++;
    }

  /* reset the redraw clipping for the next paint... */
  stage_cogl->initialized_redraw_clip = FALSE;

  /* We have repaired the backbuffer */
  stage_cogl->dirty_backbuffer = FALSE;

  stage_cogl->frame_count++;
}

static CoglFramebuffer *
clutter_stage_cogl_get_active_framebuffer (ClutterStageWindow *stage_window)
{
  return cogl_get_draw_framebuffer ();
}

static void
clutter_stage_cogl_dirty_back_buffer (ClutterStageWindow *stage_window)
{
 ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

 stage_cogl->dirty_backbuffer = TRUE;
}

static void
clutter_stage_cogl_get_dirty_pixel (ClutterStageWindow *stage_window,
                                    int                *x,
                                    int                *y)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  gboolean has_buffer_age =
    cogl_is_onscreen (stage_cogl->outputs[0].base.framebuffer) &&
    cogl_clutter_winsys_has_feature (COGL_WINSYS_FEATURE_BUFFER_AGE);

  if (!has_buffer_age)
    {
      *x = 0;
      *y = 0;
    }
  else
    {
      cairo_rectangle_int_t *rect;

      rect = &stage_cogl->damage_history[DAMAGE_HISTORY (stage_cogl->damage_index-1)];
      *x = rect->x;
      *y = rect->y;
    }
}

static void
clutter_stage_cogl_set_outputs (ClutterStageWindow *stage_window,
                                ClutterStageOutput *outputs,
                                gint                num_outputs)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  int i;

  g_free (stage_cogl->outputs);
  stage_cogl->outputs = g_new0 (ClutterStageCoglOutput, num_outputs);

  for (i = 0; i < num_outputs; i++)
    {
      ClutterStageOutput *stage_output = &stage_cogl->outputs[i].base;
      CoglFramebuffer *framebuffer;
      GError *error = NULL;

      stage_cogl->outputs[i].base = outputs[i];

      framebuffer = _clutter_stage_window_create_framebuffer (stage_window,
                                                              stage_output);

      stage_cogl->outputs[i].base.framebuffer = framebuffer;

      if (!cogl_framebuffer_allocate (framebuffer, &error))
        {
          g_warning ("Failed to allocate stage: %s", error->message);
          g_error_free (error);
          cogl_object_unref (framebuffer);
          abort();
        }
    }
  stage_cogl->num_outputs = num_outputs;

  stage_cogl->frame_closure =
    _clutter_stage_window_set_frame_callback (stage_window,
                                              frame_cb, stage_window);
}

static ClutterStageOutput *
clutter_stage_cogl_get_outputs (ClutterStageWindow *stage_window,
                                gint               *num_outputs)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  ClutterStageOutput *outputs;
  int i;

  outputs = g_new0 (ClutterStageOutput, stage_cogl->num_outputs);
  for (i = 0; i < stage_cogl->num_outputs; i++)
    outputs[i] = stage_cogl->outputs[i].base;

  *num_outputs = stage_cogl->num_outputs;
  return outputs;
}

static void
clutter_stage_window_iface_init (ClutterStageWindowIface *iface)
{
  iface->realize = clutter_stage_cogl_realize;
  iface->unrealize = clutter_stage_cogl_unrealize;
  iface->get_wrapper = clutter_stage_cogl_get_wrapper;
  iface->get_geometry = clutter_stage_cogl_get_geometry;
  iface->resize = clutter_stage_cogl_resize;
  iface->show = clutter_stage_cogl_show;
  iface->hide = clutter_stage_cogl_hide;
  iface->schedule_update = clutter_stage_cogl_schedule_update;
  iface->get_update_time = clutter_stage_cogl_get_update_time;
  iface->clear_update_time = clutter_stage_cogl_clear_update_time;
  iface->add_redraw_clip = clutter_stage_cogl_add_redraw_clip;
  iface->has_redraw_clips = clutter_stage_cogl_has_redraw_clips;
  iface->ignoring_redraw_clips = clutter_stage_cogl_ignoring_redraw_clips;
  iface->get_redraw_clip_bounds = clutter_stage_cogl_get_redraw_clip_bounds;
  iface->redraw = clutter_stage_cogl_redraw;
  iface->get_active_framebuffer = clutter_stage_cogl_get_active_framebuffer;
  iface->dirty_back_buffer = clutter_stage_cogl_dirty_back_buffer;
  iface->get_dirty_pixel = clutter_stage_cogl_get_dirty_pixel;
  iface->set_outputs = clutter_stage_cogl_set_outputs;
  iface->get_outputs = clutter_stage_cogl_get_outputs;
}

static void
clutter_stage_cogl_set_property (GObject      *gobject,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
  ClutterStageCogl *self = CLUTTER_STAGE_COGL (gobject);

  switch (prop_id)
    {
    case PROP_WRAPPER:
      self->wrapper = g_value_get_object (value);
      break;

    case PROP_BACKEND:
      self->backend = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
_clutter_stage_cogl_class_init (ClutterStageCoglClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = clutter_stage_cogl_set_property;

  g_object_class_override_property (gobject_class, PROP_WRAPPER, "wrapper");
  g_object_class_override_property (gobject_class, PROP_BACKEND, "backend");
}

static void
_clutter_stage_cogl_init (ClutterStageCogl *stage)
{
  stage->last_presentation_time = 0;
  stage->refresh_rate = 0.0;

  stage->update_time = -1;
}
