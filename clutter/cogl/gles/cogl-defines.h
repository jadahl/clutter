/*
 * Clutter COGL
 *
 * A basic GL/GLES Abstraction/Utility Layer
 *
 * Authored By Matthew Allum  <mallum@openedhand.com>
 *
 * Copyright (C) 2007 OpenedHand
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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <GLES/gl.h>

#ifndef __COGL_DEFINES_H__
#define __COGL_DEFINES_H__

G_BEGIN_DECLS

/* ClearBufferMask */
#define CGL_DEPTH_BUFFER_BIT GL_DEPTH_BUFFER_BIT
#define CGL_STENCIL_BUFFER_BIT GL_STENCIL_BUFFER_BIT
#define CGL_COLOR_BUFFER_BIT GL_COLOR_BUFFER_BIT

/* Boolean */
#define CGL_FALSE GL_FALSE
#define CGL_TRUE GL_TRUE

/* BeginMode */
#define CGL_POINTS GL_POINTS
#define CGL_LINES GL_LINES
#define CGL_LINE_LOOP GL_LINE_LOOP
#define CGL_LINE_STRIP GL_LINE_STRIP
#define CGL_TRIANGLES GL_TRIANGLES
#define CGL_TRIANGLE_STRIP GL_TRIANGLE_STRIP
#define CGL_TRIANGLE_FAN GL_TRIANGLE_FAN

/* AlphaFunction */
#define CGL_NEVER GL_NEVER
#define CGL_LESS GL_LESS
#define CGL_EQUAL GL_EQUAL
#define CGL_LEQUAL GL_LEQUAL
#define CGL_GREATER GL_GREATER
#define CGL_NOTEQUAL GL_NOTEQUAL
#define CGL_GEQUAL GL_GEQUAL
#define CGL_ALWAYS GL_ALWAYS

/* BlendingFactorDest */
#define CGL_ZERO GL_ZERO
#define CGL_ONE GL_ONE
#define CGL_SRC_COLOR GL_SRC_COLOR
#define CGL_ONE_MINUS_SRC_COLOR GL_ONE_MINUS_SRC_COLOR
#define CGL_SRC_ALPHA GL_SRC_ALPHA
#define CGL_ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define CGL_DST_ALPHA GL_DST_ALPHA
#define CGL_ONE_MINUS_DST_ALPHA GL_ONE_MINUS_DST_ALPHA

/* BlendingFactorSrc */
#define CGL_DST_COLOR GL_DST_COLOR
#define CGL_ONE_MINUS_DST_COLOR GL_ONE_MINUS_DST_COLOR
#define CGL_SRC_ALPHA_SATURATE GL_SRC_ALPHA_SATURATE
/* Missing; */
/*      GL_ZERO */
/*      GL_ONE */
/*      GL_SRC_ALPHA */
/*      GL_ONE_MINUS_SRC_ALPHA */
/*      GL_DST_ALPHA */
/*      GL_ONE_MINUS_DST_ALPHA */

/* CullFaceMode */
#define CGL_FRONT GL_FRONT
#define CGL_BACK GL_BACK
#define CGL_FRONT_AND_BACK GL_FRONT_AND_BACK

/* EnableCap */
#define CGL_FOG GL_FOG
#define CGL_LIGHTING GL_LIGHTING
#define CGL_CULL_FACE GL_CULL_FACE
#define CGL_ALPHA_TEST GL_ALPHA_TEST
#define CGL_BLEND GL_BLEND
#define CGL_COLOR_LOGIC_OP GL_COLOR_LOGIC_OP
#define CGL_DITHER GL_DITHER
#define CGL_STENCIL_TEST GL_STENCIL_TEST
#define CGL_DEPTH_TEST GL_DEPTH_TEST
#define CGL_POINT_SMOOTH GL_POINT_SMOOTH
#define CGL_LINE_SMOOTH GL_LINE_SMOOTH
#define CGL_SCISSOR_TEST GL_SCISSOR_TEST
#define CGL_COLOR_MATERIAL GL_COLOR_MATERIAL
#define CGL_NORMALIZE GL_NORMALIZE
#define CGL_RESCALE_NORMAL GL_RESCALE_NORMAL
#define CGL_POLYGON_OFFSET_FILL GL_POLYGON_OFFSET_FILL
#define CGL_VERTEX_ARRAY GL_VERTEX_ARRAY
#define CGL_NORMAL_ARRAY GL_NORMAL_ARRAY
#define CGL_COLOR_ARRAY GL_COLOR_ARRAY
#define CGL_TEXTURE_COORD_ARRAY GL_TEXTURE_COORD_ARRAY
#define CGL_MULTISAMPLE GL_MULTISAMPLE
#define CGL_SAMPLE_ALPHA_TO_COVERAGE GL_SAMPLE_ALPHA_TO_COVERAGE
#define CGL_SAMPLE_ALPHA_TO_ONE GL_SAMPLE_ALPHA_TO_ONE
#define CGL_SAMPLE_COVERAGE GL_SAMPLE_COVERAGE

/* Errors */
#define CGL_NO_ERROR GL_NO_ERROR
#define CGL_INVALID_ENUM GL_INVALID_ENUM
#define CGL_INVALID_VALUE GL_INVALID_VALUE
#define CGL_INVALID_OPERATION GL_INVALID_OPERATION
#define CGL_STACK_OVERFLOW GL_STACK_OVERFLOW
#define CGL_STACK_UNDERFLOW GL_STACK_UNDERFLOW
#define CGL_OUT_OF_MEMORY GL_OUT_OF_MEMORY

/* Fog mode */
#define CGL_EXP GL_EXP
#define CGL_EXP2 GL_EXP2
#define CGL_FOG_DENSITY GL_FOG_DENSITY

/* FogParameter */
#define CGL_FOG_START GL_FOG_START
#define CGL_FOG_END GL_FOG_END
#define CGL_FOG_MODE GL_FOG_MODE
#define CGL_FOG_COLOR GL_FOG_COLOR
#define CGL_CW GL_CW
#define CGL_CCW GL_CCW

/* GetPName */
#define CGL_CURRENT_COLOR GL_CURRENT_COLOR
#define CGL_CURRENT_NORMAL GL_CURRENT_NORMAL
#define CGL_CURRENT_TEXTURE_COORDS GL_CURRENT_TEXTURE_COORDS
#define CGL_POINT_SIZE GL_POINT_SIZE
#define CGL_POINT_SIZE_MIN GL_POINT_SIZE_MIN
#define CGL_POINT_SIZE_MAX GL_POINT_SIZE_MAX
#define CGL_POINT_FADE_THRESHOLD_SIZE GL_POINT_FADE_THRESHOLD_SIZE
#define CGL_POINT_DISTANCE_ATTENUATION GL_POINT_DISTANCE_ATTENUATION
#define CGL_SMOOTH_POINT_SIZE_RANGE GL_SMOOTH_POINT_SIZE_RANGE
#define CGL_LINE_WIDTH GL_LINE_WIDTH
#define CGL_SMOOTH_LINE_WIDTH_RANGE GL_SMOOTH_LINE_WIDTH_RANGE
#define CGL_ALIASED_POINT_SIZE_RANGE GL_ALIASED_POINT_SIZE_RANGE
#define CGL_ALIASED_LINE_WIDTH_RANGE GL_ALIASED_LINE_WIDTH_RANGE
#define CGL_CULL_FACE_MODE GL_CULL_FACE_MODE
#define CGL_FRONT_FACE GL_FRONT_FACE
#define CGL_SHADE_MODEL GL_SHADE_MODEL
#define CGL_DEPTH_RANGE GL_DEPTH_RANGE
#define CGL_DEPTH_WRITEMASK GL_DEPTH_WRITEMASK
#define CGL_DEPTH_CLEAR_VALUE GL_DEPTH_CLEAR_VALUE
#define CGL_DEPTH_FUNC GL_DEPTH_FUNC
#define CGL_STENCIL_CLEAR_VALUE GL_STENCIL_CLEAR_VALUE
#define CGL_STENCIL_FUNC GL_STENCIL_FUNC
#define CGL_STENCIL_VALUE_MASK GL_STENCIL_VALUE_MASK
#define CGL_STENCIL_FAIL GL_STENCIL_FAIL
#define CGL_STENCIL_PASS_DEPTH_FAIL GL_STENCIL_PASS_DEPTH_FAIL
#define CGL_STENCIL_PASS_DEPTH_PASS GL_STENCIL_PASS_DEPTH_PASS
#define CGL_STENCIL_REF GL_STENCIL_REF
#define CGL_STENCIL_WRITEMASK GL_STENCIL_WRITEMASK
#define CGL_MATRIX_MODE GL_MATRIX_MODE
#define CGL_VIEWPORT GL_VIEWPORT
#define CGL_MODELVIEW_STACK_DEPTH GL_MODELVIEW_STACK_DEPTH
#define CGL_PROJECTION_STACK_DEPTH GL_PROJECTION_STACK_DEPTH
#define CGL_TEXTURE_STACK_DEPTH GL_TEXTURE_STACK_DEPTH
#define CGL_MODELVIEW_MATRIX GL_MODELVIEW_MATRIX
#define CGL_PROJECTION_MATRIX GL_PROJECTION_MATRIX
#define CGL_TEXTURE_MATRIX GL_TEXTURE_MATRIX
#define CGL_ALPHA_TEST_FUNC GL_ALPHA_TEST_FUNC
#define CGL_ALPHA_TEST_REF GL_ALPHA_TEST_REF
#define CGL_BLEND_DST GL_BLEND_DST
#define CGL_BLEND_SRC GL_BLEND_SRC
#define CGL_LOGIC_OP_MODE GL_LOGIC_OP_MODE
#define CGL_SCISSOR_BOX GL_SCISSOR_BOX
#define CGL_SCISSOR_TEST GL_SCISSOR_TEST
#define CGL_COLOR_CLEAR_VALUE GL_COLOR_CLEAR_VALUE
#define CGL_COLOR_WRITEMASK GL_COLOR_WRITEMASK
#define CGL_UNPACK_ALIGNMENT GL_UNPACK_ALIGNMENT
#define CGL_PACK_ALIGNMENT GL_PACK_ALIGNMENT
#define CGL_MAX_LIGHTS GL_MAX_LIGHTS
#define CGL_MAX_CLIP_PLANES GL_MAX_CLIP_PLANES
#define CGL_MAX_TEXTURE_SIZE GL_MAX_TEXTURE_SIZE
#define CGL_MAX_MODELVIEW_STACK_DEPTH GL_MAX_MODELVIEW_STACK_DEPTH
#define CGL_MAX_PROJECTION_STACK_DEPTH GL_MAX_PROJECTION_STACK_DEPTH
#define CGL_MAX_TEXTURE_STACK_DEPTH GL_MAX_TEXTURE_STACK_DEPTH
#define CGL_MAX_VIEWPORT_DIMS GL_MAX_VIEWPORT_DIMS
#define CGL_MAX_ELEMENTS_VERTICES GL_MAX_ELEMENTS_VERTICES
#define CGL_MAX_ELEMENTS_INDICES GL_MAX_ELEMENTS_INDICES
#define CGL_MAX_TEXTURE_UNITS GL_MAX_TEXTURE_UNITS
#define CGL_SUBPIXEL_BITS GL_SUBPIXEL_BITS
#define CGL_RED_BITS GL_RED_BITS
#define CGL_GREEN_BITS GL_GREEN_BITS
#define CGL_BLUE_BITS GL_BLUE_BITS
#define CGL_ALPHA_BITS GL_ALPHA_BITS
#define CGL_DEPTH_BITS GL_DEPTH_BITS
#define CGL_STENCIL_BITS GL_STENCIL_BITS
#define CGL_POLYGON_OFFSET_UNITS GL_POLYGON_OFFSET_UNITS
#define CGL_POLYGON_OFFSET_FILL GL_POLYGON_OFFSET_FILL
#define CGL_POLYGON_OFFSET_FACTOR GL_POLYGON_OFFSET_FACTOR
#define CGL_VERTEX_ARRAY_SIZE GL_VERTEX_ARRAY_SIZE
#define CGL_VERTEX_ARRAY_TYPE GL_VERTEX_ARRAY_TYPE
#define CGL_VERTEX_ARRAY_STRIDE GL_VERTEX_ARRAY_STRIDE
#define CGL_NORMAL_ARRAY_TYPE GL_NORMAL_ARRAY_TYPE
#define CGL_NORMAL_ARRAY_STRIDE GL_NORMAL_ARRAY_STRIDE
#define CGL_COLOR_ARRAY_SIZE GL_COLOR_ARRAY_SIZE
#define CGL_COLOR_ARRAY_TYPE GL_COLOR_ARRAY_TYPE
#define CGL_COLOR_ARRAY_STRIDE GL_COLOR_ARRAY_STRIDE
#define CGL_TEXTURE_COORD_ARRAY_SIZE GL_TEXTURE_COORD_ARRAY_SIZE
#define CGL_TEXTURE_COORD_ARRAY_TYPE GL_TEXTURE_COORD_ARRAY_TYPE
#define CGL_TEXTURE_COORD_ARRAY_STRIDE GL_TEXTURE_COORD_ARRAY_STRIDE
#define CGL_VERTEX_ARRAY_POINTER GL_VERTEX_ARRAY_POINTER
#define CGL_NORMAL_ARRAY_POINTER GL_NORMAL_ARRAY_POINTER
#define CGL_COLOR_ARRAY_POINTER GL_COLOR_ARRAY_POINTER
#define CGL_TEXTURE_COORD_ARRAY_POINTER GL_TEXTURE_COORD_ARRAY_POINTER
#define CGL_SAMPLE_BUFFERS GL_SAMPLE_BUFFERS
#define CGL_SAMPLES GL_SAMPLES
#define CGL_SAMPLE_COVERAGE_VALUE GL_SAMPLE_COVERAGE_VALUE
#define CGL_SAMPLE_COVERAGE_INVERT GL_SAMPLE_COVERAGE_INVERT

/* GetTextureParameter - missing */
/*      GL_TEXTURE_MAG_FILTER */
/*      GL_TEXTURE_MIN_FILTER */
/*      GL_TEXTURE_WRAP_S */
/*      GL_TEXTURE_WRAP_T */


#define CGL_IMPLEMENTATION_COLOR_READ_TYPE_OES GL_IMPLEMENTATION_COLOR_READ_TYPE_OES
#define CGL_IMPLEMENTATION_COLOR_READ_FORMAT_OES GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES
#define CGL_NUM_COMPRESSED_TEXTURE_FORMATS GL_NUM_COMPRESSED_TEXTURE_FORMATS
#define CGL_COMPRESSED_TEXTURE_FORMATS GL_COMPRESSED_TEXTURE_FORMATS

/* OES_matrix_get */
#define CGL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS
#define CGL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS
#define CGL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS
/* HintMode */
#define CGL_DONT_CARE GL_DONT_CARE
#define CGL_FASTEST GL_FASTEST
#define CGL_NICEST GL_NICEST

/* HintTarget */
#define CGL_PERSPECTIVE_CORRECTION_HINT GL_PERSPECTIVE_CORRECTION_HINT
#define CGL_POINT_SMOOTH_HINT GL_POINT_SMOOTH_HINT
#define CGL_LINE_SMOOTH_HINT GL_LINE_SMOOTH_HINT
#define CGL_POLYGON_SMOOTH_HINT GL_POLYGON_SMOOTH_HINT
#define CGL_FOG_HINT GL_FOG_HINT
#define CGL_GENERATE_MIPMAP_HINT GL_GENERATE_MIPMAP_HINT

/* LightModelParameter */
#define CGL_LIGHT_MODEL_AMBIENT GL_LIGHT_MODEL_AMBIENT
#define CGL_LIGHT_MODEL_TWO_SIDE GL_LIGHT_MODEL_TWO_SIDE

/* LightParameter */
#define CGL_AMBIENT GL_AMBIENT
#define CGL_DIFFUSE GL_DIFFUSE
#define CGL_SPECULAR GL_SPECULAR
#define CGL_POSITION GL_POSITION
#define CGL_SPOT_DIRECTION GL_SPOT_DIRECTION
#define CGL_SPOT_EXPONENT GL_SPOT_EXPONENT
#define CGL_SPOT_CUTOFF GL_SPOT_CUTOFF
#define CGL_CONSTANT_ATTENUATION GL_CONSTANT_ATTENUATION
#define CGL_LINEAR_ATTENUATION GL_LINEAR_ATTENUATION
#define CGL_QUADRATIC_ATTENUATION GL_QUADRATIC_ATTENUATION

/* DataType */
#define CGL_BYTE GL_BYTE
#define CGL_UNSIGNED_BYTE GL_UNSIGNED_BYTE
#define CGL_SHORT GL_SHORT
#define CGL_UNSIGNED_SHORT GL_UNSIGNED_SHORT
#define CGL_FLOAT GL_FLOAT
#define CGL_FIXED GL_FIXED

/* LogicOp */
#define CGL_CLEAR GL_CLEAR
#define CGL_AND GL_AND
#define CGL_AND_REVERSE GL_AND_REVERSE
#define CGL_COPY GL_COPY
#define CGL_AND_INVERTED GL_AND_INVERTED
#define CGL_NOOP GL_NOOP
#define CGL_XOR GL_XOR
#define CGL_OR GL_OR
#define CGL_NOR GL_NOR
#define CGL_EQUIV GL_EQUIV
#define CGL_INVERT GL_INVERT
#define CGL_OR_REVERSE GL_OR_REVERSE
#define CGL_COPY_INVERTED GL_COPY_INVERTED
#define CGL_OR_INVERTED GL_OR_INVERTED
#define CGL_NAND GL_NAND
#define CGL_SET GL_SET

/* MaterialParameter */
#define CGL_EMISSION GL_EMISSION
#define CGL_SHININESS GL_SHININESS
#define CGL_AMBIENT_AND_DIFFUSE GL_AMBIENT_AND_DIFFUSE

/* MatrixMode */
#define CGL_MODELVIEW GL_MODELVIEW
#define CGL_PROJECTION GL_PROJECTION
#define CGL_TEXTURE GL_TEXTURE

/* PixelFormat */
#define CGL_ALPHA GL_ALPHA
#define CGL_RGB GL_RGB
#define CGL_RGBA GL_RGBA
#define CGL_LUMINANCE GL_LUMINANCE
#define CGL_LUMINANCE_ALPHA GL_LUMINANCE_ALPHA

/* PixelStoreParameter */
#define CGL_UNPACK_ALIGNMENT GL_UNPACK_ALIGNMENT
#define CGL_PACK_ALIGNMENT GL_PACK_ALIGNMENT

/* PixelType */
/*      GL_UNSIGNED_BYTE */
#define CGL_UNSIGNED_SHORT_4_4_4_4 GL_UNSIGNED_SHORT_4_4_4_4    
#define CGL_UNSIGNED_SHORT_5_5_5_1 GL_UNSIGNED_SHORT_5_5_5_1
#define CGL_UNSIGNED_SHORT_5_6_5   CGL_UNSIGNED_SHORT_5_6_5

/* ShadingModel */
#define CGL_FLAT GL_FLAT
#define CGL_SMOOTH GL_SMOOTH

/* StencilFunction */
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */

/* StencilOp */
#define CGL_KEEP GL_KEEP
#define CGL_REPLACE GL_REPLACE
#define CGL_INCR GL_INCR
#define CGL_DECR GL_DECR

/* StringName */
#define CGL_VENDOR GL_VENDOR
#define CGL_RENDERER GL_RENDERER
#define CGL_VERSION GL_VERSION
#define CGL_EXTENSIONS GL_EXTENSIONS

/* TextureEnvMode */
#define CGL_MODULATE GL_MODULATE
#define CGL_DECAL GL_DECAL
#define CGL_ADD GL_ADD
/*      GL_BLEND */
/*      GL_REPLACE */

/* TextureEnvParameter */
#define CGL_TEXTURE_ENV_MODE GL_TEXTURE_ENV_MODE
#define CGL_TEXTURE_ENV_COLOR GL_TEXTURE_ENV_COLOR

/* TextureEnvTarget */
#define CGL_TEXTURE_ENV GL_TEXTURE_ENV

/* TextureMagFilter */
#define CGL_NEAREST GL_NEAREST
#define CGL_LINEAR GL_LINEAR

/* TextureMinFilter */
/*      GL_NEAREST */
/*      GL_LINEAR */
#define CGL_NEAREST_MIPMAP_NEAREST GL_NEAREST_MIPMAP_NEAREST
#define CGL_LINEAR_MIPMAP_NEAREST GL_LINEAR_MIPMAP_NEAREST
#define CGL_NEAREST_MIPMAP_LINEAR GL_NEAREST_MIPMAP_LINEAR
#define CGL_LINEAR_MIPMAP_LINEAR GL_LINEAR_MIPMAP_LINEAR

/* TextureParameterName */
#define CGL_TEXTURE_MAG_FILTER GL_TEXTURE_MAG_FILTER
#define CGL_TEXTURE_MIN_FILTER GL_TEXTURE_MIN_FILTER
#define CGL_TEXTURE_WRAP_S GL_TEXTURE_WRAP_S
#define CGL_TEXTURE_WRAP_T GL_TEXTURE_WRAP_T
#define CGL_GENERATE_MIPMAP GL_GENERATE_MIPMAP

#define CGL_ACTIVE_TEXTURE GL_ACTIVE_TEXTURE
#define CGL_CLIENT_ACTIVE_TEXTURE GL_CLIENT_ACTIVE_TEXTURE

/* TextureWrapMode */
#define CGL_REPEAT GL_REPEAT
#define CGL_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE

/* PixelInternalFormat */

/* Buffer Objects */
#define CGL_ARRAY_BUFFER GL_ARRAY_BUFFER
#define CGL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER
#define CGL_ARRAY_BUFFER_BINDING GL_ARRAY_BUFFER_BINDING
#define CGL_ELEMENT_ARRAY_BUFFER_BINDING GL_ELEMENT_ARRAY_BUFFER_BINDING
#define CGL_VERTEX_ARRAY_BUFFER_BINDING GL_VERTEX_ARRAY_BUFFER_BINDING
#define CGL_NORMAL_ARRAY_BUFFER_BINDING GL_NORMAL_ARRAY_BUFFER_BINDING
#define CGL_COLOR_ARRAY_BUFFER_BINDING GL_COLOR_ARRAY_BUFFER_BINDING
#define CGL_TEXTURE_COORD_ARRAY_BUFFER_BINDING GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING
#define CGL_STATIC_DRAW GL_STATIC_DRAW
#define CGL_DYNAMIC_DRAW GL_DYNAMIC_DRAW
#define CGL_WRITE_ONLY GL_WRITE_ONLY
#define CGL_BUFFER_SIZE GL_BUFFER_SIZE
#define CGL_BUFFER_USAGE GL_BUFFER_USAGE
#define CGL_BUFFER_ACCESS GL_BUFFER_ACCESS
#define CGL_SUBTRACT GL_SUBTRACT
#define CGL_COMBINE GL_COMBINE
#define CGL_COMBINE_RGB GL_COMBINE_RGB
#define CGL_COMBINE_ALPHA GL_COMBINE_ALPHA
#define CGL_RGB_SCALE GL_RGB_SCALE
#define CGL_ADD_SIGNED GL_ADD_SIGNED
#define CGL_INTERPOLATE GL_INTERPOLATE
#define CGL_CONSTANT GL_CONSTANT
#define CGL_PRIMARY_COLOR GL_PRIMARY_COLOR
#define CGL_PREVIOUS GL_PREVIOUS
#define CGL_ALPHA_SCALE GL_ALPHA_SCALE
#define CGL_POINT_SPRITE_OES GL_POINT_SPRITE_OES
#define CGL_COORD_REPLACE_OES GL_COORD_REPLACE_OES
#define CGL_POINT_SIZE_ARRAY_OES GL_POINT_SIZE_ARRAY_OES
#define CGL_POINT_SIZE_ARRAY_TYPE_OES GL_POINT_SIZE_ARRAY_TYPE_OES
#define CGL_POINT_SIZE_ARRAY_STRIDE_OES GL_POINT_SIZE_ARRAY_STRIDE_OES
#define CGL_POINT_SIZE_ARRAY_POINTER_OES GL_POINT_SIZE_ARRAY_POINTER_OES
#define CGL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES
#define CGL_MAX_VERTEX_UNITS_OES GL_MAX_VERTEX_UNITS_OES
#define CGL_MAX_PALETTE_MATRICES_OES GL_MAX_PALETTE_MATRICES_OES
#define CGL_MATRIX_PALETTE_OES GL_MATRIX_PALETTE_OES
#define CGL_MATRIX_INDEX_ARRAY_OES GL_MATRIX_INDEX_ARRAY_OES
#define CGL_WEIGHT_ARRAY_OES GL_WEIGHT_ARRAY_OES
#define CGL_MATRIX_INDEX_ARRAY_SIZE_OES GL_MATRIX_INDEX_ARRAY_SIZE_OES
#define CGL_MATRIX_INDEX_ARRAY_TYPE_OES GL_MATRIX_INDEX_ARRAY_TYPE_OES
#define CGL_MATRIX_INDEX_ARRAY_STRIDE_OES GL_MATRIX_INDEX_ARRAY_STRIDE_OES
#define CGL_MATRIX_INDEX_ARRAY_POINTER_OES GL_MATRIX_INDEX_ARRAY_POINTER_OES
#define CGL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES
#define CGL_WEIGHT_ARRAY_SIZE_OES GL_WEIGHT_ARRAY_SIZE_OES
#define CGL_WEIGHT_ARRAY_TYPE_OES GL_WEIGHT_ARRAY_TYPE_OES
#define CGL_WEIGHT_ARRAY_STRIDE_OES GL_WEIGHT_ARRAY_STRIDE_OES
#define CGL_WEIGHT_ARRAY_POINTER_OES GL_WEIGHT_ARRAY_POINTER_OES
#define CGL_WEIGHT_ARRAY_BUFFER_BINDING_OES GL_WEIGHT_ARRAY_BUFFER_BINDING_OES
#define CGL_TEXTURE_CROP_RECT_OES GL_TEXTURE_CROP_RECT_OES

typedef GLenum COGLenum;
typedef GLint  COGLint;

/* extras */

#define CGL_TEXTURE_2D GL_TEXTURE_2D
#define CGL_ARGB GL_ARGB

/* FIXME: There is no BGR support in GLES - so with below BGR textures are 
 * borked. Will likely need a feature flag and some coversion..
 */
#define CGL_BGR GL_RGB
#define CGL_BGRA GL_RGBA

#define CGL_TEXTURE_RECTANGLE_ARB 0 /* Its unlikely we support this */

G_END_DECLS

#endif
