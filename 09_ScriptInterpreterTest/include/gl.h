#ifndef GL_H
#define GL_H

/**************************************************************************************************************
OpenGL Core bindings up to, and including, OpenGL 3.3
**************************************************************************************************************/
/*
** Copyright (c) 2013-2015 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
**
** Khronos $Revision: 31597 $ on $Date: 2015-06-25 16:32:35 -0400 (Thu, 25 Jun 2015) $
*/

#include <assert.h>
#include <iostream>
#include <cstdint>
//GL types

/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif

typedef uint64_t GLuint64;
typedef int64_t GLint64;

typedef uint32_t GLenum;
typedef unsigned char GLboolean;
typedef uint32_t GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned short GLhalf;
typedef uint32_t GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef char GLcharARB;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef struct __GLsync *GLsync;

#if _WIN32
#define NOMINMAX
#include <Windows.h>
#define GL_FUNCPTR APIENTRY
#else
#define GL_FUNCPTR APIENTRY
#endif


#define ON_LOAD_FAILED_NAME(ptr, type, name)\
	if (!ptr) {\
		numFailed++; \
		std::cout << "ERROR on line "<< __LINE__ << ": The Function <" << name << "> is not availabled on this platform\n";\
			}

#define LOAD_FUNCTION_NAME(ptr, type, name)\
    ptr = (type)GetGLProcAddress(name); \
    ON_LOAD_FAILED_NAME(ptr, type, name)

GLenum(GL_FUNCPTR glGetError)(void);


#ifdef _DEBUG
#define OPENGL_DEBUG_ERROR_LOG \
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR){\
		switch(error){ \
		case GL_INVALID_ENUM:\
			std::cout<< "GL_INVALID_ENUM Error Generated on line " << __LINE__ << std::endl;\
			break;\
		case GL_INVALID_VALUE:\
			std::cout<< "GL_INVALID_VALUE Error Generated on line " << __LINE__ << std::endl;\
			break;\
		case GL_INVALID_OPERATION:\
			std::cout<< "GL_INVALID_OPERATION Error Generated on line " << __LINE__ << std::endl;\
			break;\
		case GL_STACK_OVERFLOW:\
			std::cout<< "GL_STACK_OVERFLOW Error Generated on line " << __LINE__ << std::endl;\
			break;\
		case GL_STACK_UNDERFLOW:\
			std::cout<< "GL_STACK_UNDERFLOW Error Generated on line " << __LINE__ << std::endl;\
			break;\
		case GL_OUT_OF_MEMORY:\
			std::cout<< "GL_OUT_OF_MEMORY Error Generated on line " << __LINE__ << std::endl;\
			break;\
		default:\
			std::cout<< "UNKNOWN_ERROR Error Generated on line " << __LINE__ << "Error Code " << error << std::endl;\
			break;\
		}\
	}
#else
	#define OPENGL_DEBUG_ERROR_LOG
#endif

//TODO find better solution than endless loop

//####################################GL_CONSTANTS######################################

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT				0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT			0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT			0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT			0x83F3	

#define GL_SRGB_EXT									0x8C40
#define GL_SRGB8_EXT								0x8C41
#define GL_SRGB_ALPHA_EXT							0x8C42
#define GL_SRGB8_ALPHA8_EXT							0x8C43
#define GL_SLUMINANCE_ALPHA_EXT						0x8C44
#define GL_SLUMINANCE8_ALPHA8_EXT					0x8C45
#define GL_SLUMINANCE_EXT							0x8C46
#define GL_SLUMINANCE8_EXT							0x8C47
#define GL_COMPRESSED_SRGB_EXT						0x8C48
#define GL_COMPRESSED_SRGB_ALPHA_EXT				0x8C49
#define GL_COMPRESSED_SLUMINANCE_EXT				0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT			0x8C4B
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT			0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT		0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT		0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT		0x8C4F

//AttribMask

#define GL_CURRENT_BIT								0x00000001
#define GL_POINT_BIT								0x00000002
#define GL_LINE_BIT									0x00000004
#define GL_POLYGON_BIT								0x00000008
#define GL_POLYGON_STIPPLE_BIT						0x00000010
#define GL_PIXEL_MODE_BIT							0x00000020
#define GL_LIGHTING_BIT								0x00000040
#define GL_FOG_BIT									0x00000080
#define GL_DEPTH_BUFFER_BIT							0x00000100
#define GL_ACCUM_BUFFER_BIT							0x00000200
#define GL_STENCIL_BUFFER_BIT						0x00000400
#define GL_VIEWPORT_BIT								0x00000800
#define GL_TRANSFORM_BIT							0x00001000
#define GL_ENABLE_BIT								0x00002000
#define GL_COLOR_BUFFER_BIT							0x00004000
#define GL_HINT_BIT									0x00008000
#define GL_EVAL_BIT									0x00010000
#define GL_LIST_BIT									0x00020000
#define GL_TEXTURE_BIT								0x00040000
#define GL_SCISSOR_BIT								0x00080000
#define GL_ALL_ATTRIB_BITS							0x000FFFFF

//Matrix Mode
#define GL_MODELVIEW                                0x1700
#define GL_PROJECTION                               0x1701
#define GL_TEXTURE                                  0x1702
#define GL_COLOR                                    0x1800
#define GL_MATRIX_MODE                              0x0BA0

//Begin Mode

#define GL_POINTS									0x0000
#define GL_LINES									0x0001
#define GL_LINE_LOOP								0x0002
#define GL_LINE_STRIP								0x0003
#define GL_TRIANGLES								0x0004
#define GL_TRIANGLE_STRIP							0x0005
#define GL_TRIANGLE_FAN								0x0006
#define GL_QUADS									0x0007
#define GL_QUAD_STRIP								0x0008
#define GL_POLYGON									0x0009

//Types

#define GL_BYTE										0x1400
#define GL_UNSIGNED_BYTE							0x1401
#define GL_SHORT									0x1402
#define GL_UNSIGNED_SHORT							0x1403
#define GL_INT										0x1404
#define GL_UNSIGNED_INT								0x1405
#define GL_FLOAT									0x1406
#define GL_2_BYTES									0x1407
#define GL_3_BYTES									0x1408
#define GL_4_BYTES									0x1409
#define GL_DOUBLE									0x140A

//Bools

#define GL_FALSE									0x0000
#define GL_TRUE										0x0001

//Accumulators	
#define GL_ACCUM									0x0100
#define GL_LOAD										0x0101
#define GL_ADD										0x0104
#define GL_MULT										0x0103
#define GL_RETURN									0x0102

//Textures	
#define GL_TEXTURE0									0x84C0
#define GL_TEXTURE1									0x84C1
#define GL_TEXTURE2									0x84C2
#define GL_TEXTURE3									0x84C3
#define GL_TEXTURE4									0x84C4
#define GL_TEXTURE5									0x84C5
#define GL_TEXTURE6									0x84C6
#define GL_TEXTURE7									0x84C7
#define GL_TEXTURE8									0x84C8
#define GL_TEXTURE9									0x84C9
#define GL_TEXTURE10								0x84CA
#define GL_TEXTURE11								0x84CB
#define GL_TEXTURE12								0x84CC
#define GL_TEXTURE13								0x84CD
#define GL_TEXTURE14								0x84CE
#define GL_TEXTURE15								0x84CF
#define GL_TEXTURE16								0x84D0
#define GL_TEXTURE17								0x84D1
#define GL_TEXTURE18								0x84D2
#define GL_TEXTURE19								0x84D3
#define GL_TEXTURE20								0x84D4
#define GL_TEXTURE21								0x84D5
#define GL_TEXTURE22								0x84D6
#define GL_TEXTURE23								0x84D7
#define GL_TEXTURE24								0x84D8
#define GL_TEXTURE25								0x84D9
#define GL_TEXTURE26								0x84DA
#define GL_TEXTURE27								0x84DB
#define GL_TEXTURE28								0x84DC
#define GL_TEXTURE29								0x84DD
#define GL_TEXTURE30								0x84DE
#define GL_TEXTURE31								0x84DF

//AlphaFunc	
#define GL_NEVER									0x0200
#define GL_LESS										0x0201
#define GL_EQUAL									0x0202
#define GL_LEQUAL									0x0203
#define GL_GREATER									0x0204
#define GL_NOTEQUAL									0x0205
#define GL_GEQUAL									0x0206
#define GL_ALWAYS									0x0207

//Errors	
#define GL_NO_ERROR									0x0000
#define GL_INVALID_ENUM								0x0500
#define GL_INVALID_VALUE							0x0501
#define GL_INVALID_OPERATION						0x0502
#define GL_STACK_OVERFLOW							0x0503
#define GL_STACK_UNDERFLOW							0x0504
#define GL_OUT_OF_MEMORY							0x0505

//Gets	
#define GL_ACCUM_RED_BITS							0x0D58
#define GL_ACCUM_GREEN_BITS							0x0D59
#define GL_ACCUM_BLUE_BITS							0x0D5A
#define GL_ACCUM_ALPHA_BITS							0x0D5B
#define GL_ALPHA_TEST_FUNC							0x0BC1
#define GL_ALPHA_TEST_REF							0x0BC2
#define GL_ALPHA_TEST								0x0BC0
#define GL_TEXTURE_RESIDENT							0x8067
#define GL_ACTIVE_TEXTURE							0x84C0 
#define GL_MAX_TEXTURE_COORDS						0x8871
#define GL_MAX_TEXTURE_SIZE							0x0D33
#define GL_TEXTURE_BORDER							0x1005
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS			0x8B4D 
#define GL_VERSION									0x1F02
#define GL_TEXTURE_BINDING_1D						0x8066
#define GL_TEXTURE_BINDING_2D						0x8067
#define GL_TEXTURE_BINDING_3D						0x806A 
#define GL_CURRENT_RASTER_POSITION					0x0B07
#define GL_CURRENT_RASTER_COLOR						0x0B04
#define GL_CURRENT_RASTER_SECONDARY_COLOR			0x845F 
#define GL_CURRENT_RASTER_DISTANCE					0x0B09
#define GL_CURRENT_RASTER_INDEX						0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS			0x0B06
#define GL_CURRENT_RASTER_POSITION_VALID			0x0B08
#define GL_PIXEL_UNPACK_BUFFER_BINDING				0x88EF
#define GL_BLEND_EQUATION_RGB						0x8009
#define GL_BLEND_EQUATION_ALPHA						0x883D
#define GL_BLEND									0x0BE2
#define GL_BLEND_SRC								0x0BE1
#define GL_BLEND_DST								0x0BE0
#define GL_BLEND_COLOR								0x8005
#define GL_BUFFER_SIZE								0x8764
#define GL_BUFFER_USAGE								0x8765
#define GL_MAX_LIST_NESTING							0x0B31
#define GL_LIST_BASE								0x0B32
#define GL_DEPTH_CLEAR_VALUE						0x0B73
#define GL_ACCUM_CLEAR_VALUE						0x0B80
#define GL_INDEX_CLEAR_VALUE						0x0C20
#define GL_COLOR_CLEAR_VALUE						0x0C22
#define GL_STENCIL_CLEAR_VALUE						0x0B91
#define GL_CLIENT_ACTIVE_TEXTURE					0x84E1
#define GL_CURRENT_COLOR							0x0B00
#define GL_RGBA_MODE								0x0C31
#define GL_COLOR_WRITEMASK							0x0C23
#define GL_COLOR_MATERIAL_FACE						0x0B55
#define GL_COLOR_MATERIAL_PARAMETER					0x0B56
#define GL_COLOR_MATERIAL							0x0B57
#define GL_COLOR_ARRAY								0x8076
#define GL_COLOR_ARRAY_SIZE							0x8081
#define GL_COLOR_ARRAY_TYPE							0x8082
#define GL_COLOR_ARRAY_STRIDE						0x9083
#define GL_COLOR_ARRAY_BUFFER_BINDING				0x8898
#define GL_ARRAY_BUFFER_BINDING						0x8894
#define GL_COLOR_ARRAY_POINTER						0x8090
#define GL_COMPILE_STATUS							0x8B81
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE			0x86A0
#define GL_TEXTURE_COMPRESSED						0x86A1
#define GL_CURRENT_PROGRAM							0x8B8D
#define GL_POINT_SIZE								0x0B11
#define GL_POINT_SIZE_RANGE							0x0B12
#define GL_POINT_SIZE_GRANULARITY					0x0B13
#define GL_LINE_SMOOTH								0x0B20
#define GL_LINE_WIDTH								0x0B21
#define GL_LINE_WIDTH_RANGE							0x0B22
#define GL_LINE_WIDTH_GRANULARITY					0x0B23
#define GL_POLYGON_MODE								0x0B40
#define GL_POLYGON_SMOOTH							0x0B41
#define GL_CULL_FACE								0x0B44
#define GL_CULL_FACE_MODE							0x0B45
#define GL_FRONT_FACE								0x0B46
#define GL_DEPTH_RANGE								0x0B70
#define GL_DEPTH_TEST								0x0B71
#define GL_DEPTH_WRITEMASK							0x0B72
#define GL_DEPTH_FUNC								0x0B74
#define GL_STENCIL_TEST								0x0B90
#define GL_STENCIL_FUNC								0x0B92
#define GL_STENCIL_VALUE_MASK						0x0B93
#define GL_STENCIL_FAIL								0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL					0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS					0x0B96
#define GL_STENCIL_REF								0x0B97
#define GL_STENCIL_WRITEMASK						0x0B98
#define GL_VIEWPORT									0x0BA2
#define GL_DITHER									0x0BD0
#define GL_BLEND_DST								0x0BE0
#define GL_BLEND_SRC								0x0BE1
#define GL_BLEND									0x0BE2
#define GL_LOGIC_OP_MODE							0x0BF0
#define GL_COLOR_LOGIC_OP							0x0BF2
#define GL_DRAW_BUFFER								0x0C01
#define GL_READ_BUFFER								0x0C02
#define GL_SCISSOR_BOX								0x0C10
#define GL_SCISSOR_TEST								0x0C11
#define GL_DOUBLEBUFFER								0x0C32
#define GL_STEREO									0x0C33
#define GL_LINE_SMOOTH_HINT							0x0C52
#define GL_POLYGON_SMOOTH_HINT						0x0C53
#define GL_UNPACK_SWAP_BYTES						0x0CF0
#define GL_UNPACK_LSB_FIRST							0x0CF1
#define GL_UNPACK_ROW_LENGTH						0x0CF2
#define GL_UNPACK_SKIP_ROWS							0x0CF3
#define GL_UNPACK_SKIP_PIXELS						0x0CF4
#define GL_UNPACK_ALIGNMENT							0x0CF5
#define GL_PACK_SWAP_BYTES							0x0D00
#define GL_PACK_LSB_FIRST							0x0D01
#define GL_PACK_ROW_LENGTH							0x0D02
#define GL_PACK_SKIP_ROWS							0x0D03
#define GL_PACK_SKIP_PIXELS							0x0D04
#define GL_PACK_ALIGNMENT							0x0D05
#define GL_MAX_VIEWPORT_DIMS						0x0D3A
#define GL_SUBPIXEL_BITS							0x0D50

//Shader Types

#define GL_VERTEX_SHADER							0x8B31
#define GL_GEOMETRY_SHADER							0x8DD9
#define GL_TESS_EVALUATION_SHADER					0x8E87
#define GL_TESS_CONTROL_SHADER						0x8E88
#define GL_FRAGMENT_SHADER							0x8B30
#define GL_COMPUTE_SHADER							0x91B9
//Texture internal formats

#define GL_COMPRESSED_ALPHA							0x84E9
#define GL_COMPRESSED_LUMINANCE						0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA				0x84EB
#define GL_COMPRESSED_INTENSITY						0x84EC
#define GL_COMPRESSED_RGB							0x84ED
#define GL_COMPRESSED_RGBA							0x84EE
#define GL_DEPTH_COMPONENT16						0x81A5
#define GL_DEPTH_COMPONENT24						0x81A6
#define GL_DEPTH_COMPONENT32						0x81A7
#define GL_DEPTH_COMPONENT							0x1902
#define GL_RGB										0x1907
#define GL_ALPHA									0x1906
#define GL_LUMINANCE								0x1909
#define GL_LUMINANCE_ALPHA							0x190A
#define GL_R3_G3_B2									0x2A10
#define GL_ALPHA4									0x803B
#define GL_ALPHA8									0x803C
#define GL_ALPHA12									0x803D
#define GL_ALPHA16									0x803E
#define GL_LUMINANCE4								0x803F
#define GL_LUMINANCE8								0x8040
#define GL_LUMINANCE12								0x8041
#define GL_LUMINANCE16								0x8042
#define GL_LUMINANCE4_ALPHA4						0x8043
#define GL_LUMINANCE6_ALPHA2						0x8044
#define GL_LUMINANCE8_ALPHA8						0x8045
#define GL_LUMINANCE12_ALPHA4						0x8046
#define GL_LUMINANCE12_ALPHA12						0x8047
#define GL_LUMINANCE16_ALPHA16						0x8048
#define GL_INTENSITY								0x8049
#define GL_INTENSITY4								0x804A
#define GL_INTENSITY8								0x804B
#define GL_INTENSITY12								0x804C
#define GL_INTENSITY16								0x804D
#define GL_RGB4										0x804F
#define GL_RGB5										0x8050
#define GL_RGB8										0x8051
#define GL_RGB10									0x8052
#define GL_RGB12									0x8053
#define GL_RGB16									0x8054
#define GL_RGBA2									0x8055
#define GL_RGBA4									0x8056
#define GL_RGB5_A1									0x8057
#define GL_RGBA8									0x8058
#define GL_RGB10_A2									0x8059
#define GL_RGBA12									0x805A
#define GL_RGBA16									0x805B
#define GL_BGR										0x80E0
#define GL_BGRA										0x80E1
#define GL_SRGB										0x8C40
#define GL_SRGB8									0x8C41
#define GL_SRGB_ALPHA								0x8C42
#define GL_SRGB8_ALPHA8								0x8C43
#define GL_SLUMINANCE_ALPHA							0x8C44
#define GL_SLUMINANCE8_ALPHA8						0x8C45
#define GL_SLUMINANCE								0x8C46
#define GL_SLUMINANCE8								0x8C47
//GL face

#define GL_FRONT_LEFT								0x0400
#define GL_FRONT_RIGHT								0x0401
#define GL_BACK_LEFT								0x0402
#define GL_BACK_RIGHT								0x0403
#define GL_FRONT									0x0404
#define GL_BACK										0x0405
#define GL_LEFT										0x0406
#define GL_RIGHT									0x0407
#define GL_FRONT_AND_BACK							0x0408

//Color matrial mode

#define GL_EMISSION									0x1600
#define GL_AMBIENT									0x1200
#define GL_DIFFUSE									0x1201
#define GL_SPECULAR									0x1202
#define GL_AMBIENT_AND_DIFFUSE						0x1603


//GL Blend Equation
#define GL_FUNC_ADD									0x8006
#define GL_MIN										0x8007
#define GL_MAX										0x8008
#define GL_FUNC_SUBTRACT							0x800A
#define GL_FUNC_REVERSE_SUBTRACT					0x800B

//GL Clip plane
#define GL_CLIP_PLANE0								0x3000

//GL Blend Func
#define GL_ZERO										0
#define GL_ONE										1
#define GL_SRC_COLOR								0x0300
#define GL_ONE_MINUS_SRC_COLOR						0x0301
#define GL_SRC_ALPHA								0x0302
#define GL_ONE_MINUS_SRC_ALPHA						0x0303
#define GL_DST_ALPHA								0x0304
#define GL_ONE_MINUS_DST_ALPHA						0x0305
#define GL_DST_COLOR								0x0306
#define GL_ONE_MINUS_DST_COLOR						0x0307
#define GL_SRC_ALPHA_SATURATE						0x0308
#define GL_CONSTANT_COLOR							0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR					0x8002
#define GL_CONSTANT_ALPHA							0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA					0x8004

//GL Buffer Data	
#define GL_STREAM_DRAW								0x88E0
#define GL_STREAM_READ								0x88E1
#define GL_STREAM_COPY								0x88E2
#define GL_STATIC_DRAW								0x88E4
#define GL_STATIC_READ								0x88E5
#define GL_STATIC_COPY								0x88E6
#define GL_DYNAMIC_DRAW								0x88E8
#define GL_DYNAMIC_READ								0x88E9
#define GL_DYNAMIC_COPY								0x88EA

#define GL_SAMPLES_PASSED							0x8914


//GL Buffers
#define GL_ARRAY_BUFFER								0x8892
#define GL_ELEMENT_ARRAY_BUFFER						0x8893
#define GL_PIXEL_PACK_BUFFER						0x88EB
#define GL_PIXEL_UNPACK_BUFFER						0x88EC

//GL Texture Target
#define GL_PROXY_TEXTURE_1D							0x8063
#define GL_TEXTURE_1D								0x0DE0
#define GL_PROXY_TEXTURE_2D							0x8064
#define GL_TEXTURE_2D								0x0DE1
#define GL_TEXTURE_3D								0x806F
#define GL_PROXY_TEXTURE_3D							0x8070
#define GL_TEXTURE_CUBE_MAP							0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X				0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X				0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y				0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y				0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z				0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z				0x851A

//OpenGL 1.1

#define GL_NONE										0

//Front Face Direction

#define GL_CW										0x0900
#define GL_CCW										0x0901

// Polygon Offset

#define GL_POLYGON_OFFSET_UNITS						0x2A00
#define GL_POLYGON_OFFSET_POINT						0x2A01
#define GL_POLYGON_OFFSET_LINE						0x2A02
#define GL_POLYGON_OFFSET_FILL						0x8037
#define GL_POLYGON_OFFSET_FACTOR					0x8038

//GetTexParameters

#define GL_TEXTURE_WIDTH							0x1000
#define GL_TEXTURE_HEIGHT							0x1001
#define GL_TEXTURE_INTERNAL_FORMAT					0x1003
#define GL_TEXTURE_BORDER_COLOR						0x1004
#define GL_TEXTURE_RED_SIZE							0x805C
#define GL_TEXTURE_GREEN_SIZE						0x805D
#define GL_TEXTURE_BLUE_SIZE						0x805E
#define GL_TEXTURE_ALPHA_SIZE						0x805F

//Hint

#define GL_DONT_CARE								0x1100
#define GL_FASTEST									0x1101
#define GL_NICEST									0x1102

//Logic

#define GL_CLEAR									0x1500
#define GL_AND										0x1501
#define GL_AND_REVERSE								0x1502
#define GL_COPY										0x1503
#define GL_AND_INVERTED								0x1504
#define GL_NOOP										0x1505
#define GL_XOR										0x1506
#define GL_OR										0x1507
#define GL_NOR										0x1508
#define GL_EQUIV									0x1509
#define GL_INVERT									0x150A
#define GL_OR_REVERSE								0x150B
#define GL_COPY_INVERTED							0x150C
#define GL_OR_INVERTED								0x150D
#define GL_NAND										0x150E
#define GL_SET										0x150F

//Matrix

#define GL_TEXTURE									0x1702

//Pixel Copy

#define GL_COLOR									0x1800
#define GL_DEPTH									0x1801
#define GL_STENCIL									0x1802

//Pixel Format

#define GL_STENCIL_INDEX							0x1901
#define GL_DEPTH_COMPONENT							0x1902
#define GL_RED										0x1903
#define GL_GREEN									0x1904
#define GL_BLUE										0x1905
#define GL_ALPHA									0x1906
#define GL_RGB										0x1907
#define GL_RGBA										0x1908

//PolygonMode

#define GL_POINT									0x1B00
#define GL_LINE										0x1B01
#define GL_FILL										0x1B02

//Stencil Operations

#define GL_KEEP										0x1E00
#define GL_REPLACE									0x1E01
#define GL_INCR										0x1E02
#define GL_DECR										0x1E03

//GetString

#define GL_VENDOR									0x1F00
#define GL_RENDERER									0x1F01
#define GL_VERSION									0x1F02
#define GL_EXTENSIONS								0x1F03

//Texture Filter

#define GL_NEAREST									0x2600
#define GL_LINEAR									0x2601
//Texture MinFilter

#define GL_NEAREST_MIPMAP_NEAREST					0x2700
#define GL_LINEAR_MIPMAP_NEAREST					0x2701
#define GL_NEAREST_MIPMAP_LINEAR					0x2702
#define GL_LINEAR_MIPMAP_LINEAR						0x2703

//Tex Parameter Name
#define GL_TEXTURE_MAG_FILTER						0x2800
#define GL_TEXTURE_MIN_FILTER						0x2801
#define GL_TEXTURE_WRAP_S							0x2802
#define GL_TEXTURE_WRAP_T							0x2803

//Wrap Mode

#define GL_REPEAT									0x2901

//Vertex Array
#define GL_VERTEX_ARRAY								0x8074

// TODO sort this mess

#define GL_UNSIGNED_BYTE_3_3_2						0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4					0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1					0x8034
#define GL_UNSIGNED_INT_8_8_8_8						0x8035
#define GL_UNSIGNED_INT_10_10_10_2					0x8036
#define GL_TEXTURE_BINDING_3D						0x806A
#define GL_PACK_SKIP_IMAGES							0x806B
#define GL_PACK_IMAGE_HEIGHT						0x806C
#define GL_UNPACK_SKIP_IMAGES						0x806D
#define GL_UNPACK_IMAGE_HEIGHT						0x806E
#define GL_TEXTURE_DEPTH							0x8071
#define GL_TEXTURE_WRAP_R							0x8072
#define GL_MAX_3D_TEXTURE_SIZE						0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV					0x8362
#define GL_UNSIGNED_SHORT_5_6_5						0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV					0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV				0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV				0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV					0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV				0x8368
#define GL_MAX_ELEMENTS_VERTICES					0x80E8
#define GL_MAX_ELEMENTS_INDICES						0x80E9
#define GL_CLAMP_TO_EDGE							0x812F
#define GL_TEXTURE_MIN_LOD							0x813A
#define GL_TEXTURE_MAX_LOD							0x813B
#define GL_TEXTURE_BASE_LEVEL						0x813C
#define GL_TEXTURE_MAX_LEVEL						0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE					0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY			0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE					0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY			0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE					0x846E


#define GL_MULTISAMPLE								0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE					0x809E
#define GL_SAMPLE_ALPHA_TO_ONE						0x809F
#define GL_SAMPLE_COVERAGE							0x80A0
#define GL_SAMPLE_BUFFERS							0x80A8
#define GL_SAMPLES									0x80A9
#define GL_SAMPLE_COVERAGE_VALUE					0x80AA
#define GL_SAMPLE_COVERAGE_INVERT					0x80AB
#define GL_TEXTURE_BINDING_CUBE_MAP					0x8514
#define GL_PROXY_TEXTURE_CUBE_MAP					0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE				0x851C
#define GL_TEXTURE_COMPRESSION_HINT					0x84EF
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS			0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS				0x86A3
#define GL_CLAMP_TO_BORDER							0x812D

#define GL_BLEND_DST_RGB							0x80C8
#define GL_BLEND_SRC_RGB							0x80C9
#define GL_BLEND_DST_ALPHA							0x80CA
#define GL_BLEND_SRC_ALPHA							0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE				0x8128
#define GL_MIRRORED_REPEAT							0x8370
#define GL_MAX_TEXTURE_LOD_BIAS						0x84FD
#define GL_TEXTURE_LOD_BIAS							0x8501
#define GL_INCR_WRAP								0x8507
#define GL_DECR_WRAP								0x8508
#define GL_TEXTURE_DEPTH_SIZE						0x884A
#define GL_TEXTURE_COMPARE_MODE						0x884C
#define GL_TEXTURE_COMPARE_FUNC						0x884D

#define GL_QUERY_COUNTER_BITS						0x8864
#define GL_CURRENT_QUERY							0x8865
#define GL_QUERY_RESULT								0x8866
#define GL_QUERY_RESULT_AVAILABLE					0x8867
#define GL_ELEMENT_ARRAY_BUFFER_BINDING				0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING		0x889F
#define GL_READ_ONLY								0x88B8
#define GL_WRITE_ONLY								0x88B9
#define GL_READ_WRITE								0x88BA
#define GL_BUFFER_ACCESS							0x88BB
#define GL_BUFFER_MAPPED							0x88BC
#define GL_BUFFER_MAP_POINTER						0x88BD
#define GL_SRC1_ALPHA								0x8589


#define GL_VERTEX_ATTRIB_ARRAY_ENABLED				0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       			0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     			0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       			0x8625
#define GL_CURRENT_VERTEX_ATTRIB          			0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      			0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    			0x8645
#define GL_STENCIL_BACK_FUNC              			0x8800
#define GL_STENCIL_BACK_FAIL              			0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   			0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   			0x8803
#define GL_MAX_DRAW_BUFFERS               			0x8824
#define GL_DRAW_BUFFER0                   			0x8825
#define GL_DRAW_BUFFER1                   			0x8826
#define GL_DRAW_BUFFER2                   			0x8827
#define GL_DRAW_BUFFER3                   			0x8828
#define GL_DRAW_BUFFER4                   			0x8829
#define GL_DRAW_BUFFER5                   			0x882A
#define GL_DRAW_BUFFER6                   			0x882B
#define GL_DRAW_BUFFER7                   			0x882C
#define GL_DRAW_BUFFER8                   			0x882D
#define GL_DRAW_BUFFER9                   			0x882E
#define GL_DRAW_BUFFER10                  			0x882F
#define GL_DRAW_BUFFER11                  			0x8830
#define GL_DRAW_BUFFER12                  			0x8831
#define GL_DRAW_BUFFER13                  			0x8832
#define GL_DRAW_BUFFER14                  			0x8833
#define GL_DRAW_BUFFER15                  			0x8834
#define GL_MAX_VERTEX_ATTRIBS             			0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 			0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS        			0x8872
#define GL_FRAGMENT_SHADER                			0x8B30
#define GL_VERTEX_SHADER                  			0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 			0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  			0x8B4A
#define GL_MAX_VARYING_FLOATS             			0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 			0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 		0x8B4D
#define GL_SHADER_TYPE                    			0x8B4F
#define GL_FLOAT_VEC2                     			0x8B50
#define GL_FLOAT_VEC3                     			0x8B51
#define GL_FLOAT_VEC4                     			0x8B52
#define GL_INT_VEC2                       			0x8B53
#define GL_INT_VEC3                       			0x8B54
#define GL_INT_VEC4                       			0x8B55
#define GL_BOOL                           			0x8B56
#define GL_BOOL_VEC2                      			0x8B57
#define GL_BOOL_VEC3                      			0x8B58
#define GL_BOOL_VEC4                      			0x8B59
#define GL_FLOAT_MAT2                     			0x8B5A
#define GL_FLOAT_MAT3                     			0x8B5B
#define GL_FLOAT_MAT4                     			0x8B5C
#define GL_SAMPLER_1D                     			0x8B5D
#define GL_SAMPLER_2D                     			0x8B5E
#define GL_SAMPLER_3D                     			0x8B5F
#define GL_SAMPLER_CUBE                   			0x8B60
#define GL_SAMPLER_1D_SHADOW              			0x8B61
#define GL_SAMPLER_2D_SHADOW              			0x8B62
#define GL_DELETE_STATUS                  			0x8B80
#define GL_LINK_STATUS                    			0x8B82
#define GL_VALIDATE_STATUS                			0x8B83
#define GL_INFO_LOG_LENGTH                			0x8B84
#define GL_ATTACHED_SHADERS               			0x8B85
#define GL_ACTIVE_UNIFORMS                			0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      			0x8B87
#define GL_SHADER_SOURCE_LENGTH           			0x8B88
#define GL_ACTIVE_ATTRIBUTES              			0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    			0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 			0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       			0x8B8C
#define GL_POINT_SPRITE_COORD_ORIGIN      			0x8CA0
#define GL_LOWER_LEFT                     			0x8CA1
#define GL_UPPER_LEFT                     			0x8CA2
#define GL_STENCIL_BACK_REF               			0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        			0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         			0x8CA5


#define GL_COMPARE_REF_TO_TEXTURE         			0x884E
#define GL_CLIP_DISTANCE0                 			0x3000
#define GL_CLIP_DISTANCE1                 			0x3001
#define GL_CLIP_DISTANCE2                 			0x3002
#define GL_CLIP_DISTANCE3                 			0x3003
#define GL_CLIP_DISTANCE4                 			0x3004
#define GL_CLIP_DISTANCE5                 			0x3005
#define GL_CLIP_DISTANCE6                 			0x3006
#define GL_CLIP_DISTANCE7                 			0x3007
#define GL_MAX_CLIP_DISTANCES             			0x0D32
#define GL_MAJOR_VERSION                  			0x821B
#define GL_MINOR_VERSION                  			0x821C
#define GL_NUM_EXTENSIONS                 			0x821D
#define GL_CONTEXT_FLAGS                  			0x821E
#define GL_COMPRESSED_RED                 			0x8225
#define GL_COMPRESSED_RG                  			0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT		0x00000001
#define GL_RGBA32F                        			0x8814
#define GL_RGB32F                         			0x8815
#define GL_RGBA16F                        			0x881A
#define GL_RGB16F                         			0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    			0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       			0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       			0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       			0x8905
#define GL_CLAMP_READ_COLOR               			0x891C
#define GL_FIXED_ONLY                     			0x891D
#define GL_MAX_VARYING_COMPONENTS         			0x8B4B
#define GL_TEXTURE_1D_ARRAY               			0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY         			0x8C19
#define GL_TEXTURE_2D_ARRAY               			0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY         			0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY       			0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY       			0x8C1D
#define GL_R11F_G11F_B10F                 			0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   			0x8C3B
#define GL_RGB9_E5                        			0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       			0x8C3E
#define GL_TEXTURE_SHARED_SIZE            			0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 	0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 			0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    			0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 			0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 			0x8C85
#define GL_PRIMITIVES_GENERATED           			0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 	0x8C88
#define GL_RASTERIZER_DISCARD             			0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS	0x8C8B
#define GL_INTERLEAVED_ATTRIBS            			0x8C8C
#define GL_SEPARATE_ATTRIBS               			0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      			0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING		0x8C8F
#define GL_RGBA32UI                       			0x8D70
#define GL_RGB32UI                        			0x8D71
#define GL_RGBA16UI                       			0x8D76
#define GL_RGB16UI                        			0x8D77
#define GL_RGBA8UI                        			0x8D7C
#define GL_RGB8UI                         			0x8D7D
#define GL_RGBA32I                        			0x8D82
#define GL_RGB32I                         			0x8D83
#define GL_RGBA16I                        			0x8D88
#define GL_RGB16I                         			0x8D89
#define GL_RGBA8I                         			0x8D8E
#define GL_RGB8I                          			0x8D8F
#define GL_RED_INTEGER                    			0x8D94
#define GL_GREEN_INTEGER                  			0x8D95
#define GL_BLUE_INTEGER                   			0x8D96
#define GL_RGB_INTEGER                    			0x8D98
#define GL_RGBA_INTEGER                   			0x8D99
#define GL_BGR_INTEGER                    			0x8D9A
#define GL_BGRA_INTEGER                   			0x8D9B
#define GL_SAMPLER_1D_ARRAY               			0x8DC0
#define GL_SAMPLER_2D_ARRAY               			0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW        			0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW        			0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            			0x8DC5
#define GL_UNSIGNED_INT_VEC2              			0x8DC6
#define GL_UNSIGNED_INT_VEC3              			0x8DC7
#define GL_UNSIGNED_INT_VEC4              			0x8DC8
#define GL_INT_SAMPLER_1D                 			0x8DC9
#define GL_INT_SAMPLER_2D                 			0x8DCA
#define GL_INT_SAMPLER_3D                 			0x8DCB
#define GL_INT_SAMPLER_CUBE               			0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY           			0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY           			0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D        			0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D        			0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D       			0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      			0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY  			0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  			0x8DD7
#define GL_QUERY_WAIT                     			0x8E13
#define GL_QUERY_NO_WAIT                  			0x8E14
#define GL_QUERY_BY_REGION_WAIT           			0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT        			0x8E16
#define GL_BUFFER_ACCESS_FLAGS            			0x911F
#define GL_BUFFER_MAP_LENGTH              			0x9120
#define GL_BUFFER_MAP_OFFSET              			0x9121
#define GL_DEPTH_COMPONENT32F             			0x8CAC
#define GL_DEPTH32F_STENCIL8              			0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 			0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION  			0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING	0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE	0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE			0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE		0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE			0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE		0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE		0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE		0x8217
#define GL_FRAMEBUFFER_DEFAULT            			0x8218
#define GL_FRAMEBUFFER_UNDEFINED          			0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       			0x821A
#define GL_MAX_RENDERBUFFER_SIZE          			0x84E8
#define GL_DEPTH_STENCIL                  			0x84F9
#define GL_UNSIGNED_INT_24_8              			0x84FA
#define GL_DEPTH24_STENCIL8               			0x88F0
#define GL_TEXTURE_STENCIL_SIZE           			0x88F1
#define GL_TEXTURE_RED_TYPE               			0x8C10
#define GL_TEXTURE_GREEN_TYPE             			0x8C11
#define GL_TEXTURE_BLUE_TYPE              			0x8C12
#define GL_TEXTURE_ALPHA_TYPE             			0x8C13
#define GL_TEXTURE_DEPTH_TYPE             			0x8C16
#define GL_UNSIGNED_NORMALIZED            			0x8C17
#define GL_FRAMEBUFFER_BINDING            			0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       			0x8CA6
#define GL_RENDERBUFFER_BINDING           			0x8CA7
#define GL_READ_FRAMEBUFFER               			0x8CA8
#define GL_DRAW_FRAMEBUFFER               			0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       			0x8CAA
#define GL_RENDERBUFFER_SAMPLES           			0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE		0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME		0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL		0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER		0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           			0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT		0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER		0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER		0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        			0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          			0x8CDF
#define GL_COLOR_ATTACHMENT0              			0x8CE0
#define GL_COLOR_ATTACHMENT1              			0x8CE1
#define GL_COLOR_ATTACHMENT2              			0x8CE2
#define GL_COLOR_ATTACHMENT3              			0x8CE3
#define GL_COLOR_ATTACHMENT4              			0x8CE4
#define GL_COLOR_ATTACHMENT5              			0x8CE5
#define GL_COLOR_ATTACHMENT6              			0x8CE6
#define GL_COLOR_ATTACHMENT7              			0x8CE7
#define GL_COLOR_ATTACHMENT8              			0x8CE8
#define GL_COLOR_ATTACHMENT9              			0x8CE9
#define GL_COLOR_ATTACHMENT10             			0x8CEA
#define GL_COLOR_ATTACHMENT11             			0x8CEB
#define GL_COLOR_ATTACHMENT12             			0x8CEC
#define GL_COLOR_ATTACHMENT13             			0x8CED
#define GL_COLOR_ATTACHMENT14             			0x8CEE
#define GL_COLOR_ATTACHMENT15             			0x8CEF
#define GL_COLOR_ATTACHMENT16             			0x8CF0
#define GL_COLOR_ATTACHMENT17             			0x8CF1
#define GL_COLOR_ATTACHMENT18             			0x8CF2
#define GL_COLOR_ATTACHMENT19             			0x8CF3
#define GL_COLOR_ATTACHMENT20             			0x8CF4
#define GL_COLOR_ATTACHMENT21             			0x8CF5
#define GL_COLOR_ATTACHMENT22             			0x8CF6
#define GL_COLOR_ATTACHMENT23             			0x8CF7
#define GL_COLOR_ATTACHMENT24             			0x8CF8
#define GL_COLOR_ATTACHMENT25             			0x8CF9
#define GL_COLOR_ATTACHMENT26             			0x8CFA
#define GL_COLOR_ATTACHMENT27             			0x8CFB
#define GL_COLOR_ATTACHMENT28             			0x8CFC
#define GL_COLOR_ATTACHMENT29             			0x8CFD
#define GL_COLOR_ATTACHMENT30             			0x8CFE
#define GL_COLOR_ATTACHMENT31             			0x8CFF
#define GL_DEPTH_ATTACHMENT               			0x8D00
#define GL_STENCIL_ATTACHMENT             			0x8D20
#define GL_FRAMEBUFFER                    			0x8D40
#define GL_RENDERBUFFER                   			0x8D41
#define GL_RENDERBUFFER_WIDTH             			0x8D42
#define GL_RENDERBUFFER_HEIGHT            			0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   			0x8D44
#define GL_STENCIL_INDEX1                 			0x8D46
#define GL_STENCIL_INDEX4                 			0x8D47
#define GL_STENCIL_INDEX8                 			0x8D48
#define GL_STENCIL_INDEX16                			0x8D49
#define GL_RENDERBUFFER_RED_SIZE          			0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        			0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         			0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        			0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        			0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      			0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE		0x8D56
#define GL_MAX_SAMPLES                    			0x8D57
#define GL_FRAMEBUFFER_SRGB               			0x8DB9
#define GL_HALF_FLOAT                     			0x140B
#define GL_MAP_READ_BIT                   			0x0001
#define GL_MAP_WRITE_BIT                  			0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       			0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      			0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         			0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         			0x0020
#define GL_COMPRESSED_RED_RGTC1           			0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1    			0x8DBC
#define GL_COMPRESSED_RG_RGTC2            			0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2     			0x8DBE
#define GL_RG                             			0x8227
#define GL_RG_INTEGER                     			0x8228
#define GL_R8                             			0x8229
#define GL_R16                            			0x822A
#define GL_RG8                            			0x822B
#define GL_RG16                           			0x822C
#define GL_R16F                           			0x822D
#define GL_R32F                           			0x822E
#define GL_RG16F                          			0x822F
#define GL_RG32F                          			0x8230
#define GL_R8I                            			0x8231
#define GL_R8UI                           			0x8232
#define GL_R16I                           			0x8233
#define GL_R16UI                          			0x8234
#define GL_R32I                           			0x8235
#define GL_R32UI                          			0x8236
#define GL_RG8I                           			0x8237
#define GL_RG8UI                          			0x8238
#define GL_RG16I                          			0x8239
#define GL_RG16UI                         			0x823A
#define GL_RG32I                          			0x823B
#define GL_RG32UI                         			0x823C
#define GL_VERTEX_ARRAY_BINDING           			0x85B5


#define GL_SAMPLER_2D_RECT                			0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW         			0x8B64
#define GL_SAMPLER_BUFFER                 			0x8DC2
#define GL_INT_SAMPLER_2D_RECT            			0x8DCD
#define GL_INT_SAMPLER_BUFFER             			0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT   			0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER    			0x8DD8
#define GL_TEXTURE_BUFFER                 			0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE        			0x8C2B
#define GL_TEXTURE_BINDING_BUFFER         			0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 		0x8C2D
#define GL_TEXTURE_RECTANGLE              			0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE      			0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE        			0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE     			0x84F8
#define GL_R8_SNORM                       			0x8F94
#define GL_RG8_SNORM                      			0x8F95
#define GL_RGB8_SNORM                     			0x8F96
#define GL_RGBA8_SNORM                    			0x8F97
#define GL_R16_SNORM                      			0x8F98
#define GL_RG16_SNORM                     			0x8F99
#define GL_RGB16_SNORM                    			0x8F9A
#define GL_RGBA16_SNORM                   			0x8F9B
#define GL_SIGNED_NORMALIZED              			0x8F9C
#define GL_PRIMITIVE_RESTART              			0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX        			0x8F9E
#define GL_COPY_READ_BUFFER               			0x8F36
#define GL_COPY_WRITE_BUFFER              			0x8F37
#define GL_UNIFORM_BUFFER                 			0x8A11
#define GL_UNIFORM_BUFFER_BINDING         			0x8A28
#define GL_UNIFORM_BUFFER_START           			0x8A29
#define GL_UNIFORM_BUFFER_SIZE            			0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      			0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS    			0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    			0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    			0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    			0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         			0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS	0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 			0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH		0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          			0x8A36
#define GL_UNIFORM_TYPE                   			0x8A37
#define GL_UNIFORM_SIZE                   			0x8A38
#define GL_UNIFORM_NAME_LENGTH            			0x8A39
#define GL_UNIFORM_BLOCK_INDEX            			0x8A3A
#define GL_UNIFORM_OFFSET                 			0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           			0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          			0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           			0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          			0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        			0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      			0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  			0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES		0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  			0xFFFFFFFFu



#define GL_CONTEXT_CORE_PROFILE_BIT       			0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 		0x00000002
#define GL_LINES_ADJACENCY                			0x000A
#define GL_LINE_STRIP_ADJACENCY           			0x000B
#define GL_TRIANGLES_ADJACENCY            			0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY       			0x000D
#define GL_PROGRAM_POINT_SIZE             			0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 		0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 			0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS		0x8DA8
#define GL_GEOMETRY_SHADER                			0x8DD9
#define GL_GEOMETRY_VERTICES_OUT          			0x8916
#define GL_GEOMETRY_INPUT_TYPE            			0x8917
#define GL_GEOMETRY_OUTPUT_TYPE           			0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 			0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES   			0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS		0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   			0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS  			0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 			0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  			0x9125
#define GL_CONTEXT_PROFILE_MASK           			0x9126
#define GL_DEPTH_CLAMP                    			0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION        			0x8E4D
#define GL_LAST_VERTEX_CONVENTION         			0x8E4E
#define GL_PROVOKING_VERTEX               			0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS      			0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT        			0x9111
#define GL_OBJECT_TYPE                    			0x9112
#define GL_SYNC_CONDITION                 			0x9113
#define GL_SYNC_STATUS                    			0x9114
#define GL_SYNC_FLAGS                     			0x9115
#define GL_SYNC_FENCE                     			0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     			0x9117
#define GL_UNSIGNALED                     			0x9118
#define GL_SIGNALED                       			0x9119
#define GL_ALREADY_SIGNALED               			0x911A
#define GL_TIMEOUT_EXPIRED                			0x911B
#define GL_CONDITION_SATISFIED            			0x911C
#define GL_WAIT_FAILED                    			0x911D
#define GL_TIMEOUT_IGNORED                			0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_FLUSH_COMMANDS_BIT        			0x00000001
#define GL_SAMPLE_POSITION                			0x8E50
#define GL_SAMPLE_MASK                    			0x8E51
#define GL_SAMPLE_MASK_VALUE              			0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS          			0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE         			0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE   			0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY   			0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY		0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 			0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY		0x9105
#define GL_TEXTURE_SAMPLES                			0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 			0x9107
#define GL_SAMPLER_2D_MULTISAMPLE         			0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE     			0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE		0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY   			0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 		0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES      			0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES      			0x910F
#define GL_MAX_INTEGER_SAMPLES            			0x9110


#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR    			0x88FE
#define GL_SRC1_COLOR                     			0x88F9
#define GL_ONE_MINUS_SRC1_COLOR           			0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA           			0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS   			0x88FC
#define GL_ANY_SAMPLES_PASSED             			0x8C2F
#define GL_SAMPLER_BINDING                			0x8919
#define GL_RGB10_A2UI                     			0x906F
#define GL_TEXTURE_SWIZZLE_R              			0x8E42
#define GL_TEXTURE_SWIZZLE_G              			0x8E43
#define GL_TEXTURE_SWIZZLE_B              			0x8E44
#define GL_TEXTURE_SWIZZLE_A              			0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA           			0x8E46
#define GL_TIME_ELAPSED                   			0x88BF
#define GL_TIMESTAMP                      			0x8E28
#define GL_INT_2_10_10_10_REV             			0x8D9F

// WGL_CONSTANTS

#define WGL_CONTEXT_DEBUG_BIT_ARB					0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB		0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB			0x0001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB	0x0002
#define WGL_DRAW_TO_WINDOW_ARB						0x2001
#define WGL_ACCELERATION_ARB						0x2003
#define WGL_SUPPORT_OPENGL_ARB						0x2010
#define WGL_DOUBLE_BUFFER_ARB						0x2011
#define WGL_STEREO_ARB								0x2012
#define WGL_PIXEL_TYPE_ARB							0x2013
#define WGL_COLOR_BITS_ARB							0x2014
#define WGL_ACCUM_ALPHA_BITS_ARB					0x2021
#define WGL_DEPTH_BITS_ARB							0x2022
#define WGL_STENCIL_BITS_ARB						0x2023
#define WGL_FULL_ACCELERATION_ARB					0x2027
#define WGL_TYPE_RGBA_ARB							0x202B
#define WGL_SAMPLE_BUFFERS_ARB						0x2041
#define WGL_SAMPLES_ARB								0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB				0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB				0x2092
#define WGL_CONTEXT_FLAGS_ARB						0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB				0x9126



//####################################GL_FUNCTIONS######################################
//OpenGL1.0

extern void (GL_FUNCPTR* _ptr_glCullFace)(GLenum mode);
extern void (GL_FUNCPTR* _ptr_glFrontFace)(GLenum mode);
extern void (GL_FUNCPTR* _ptr_glHint)(GLenum target, GLenum mode);
extern void (GL_FUNCPTR* _ptr_glLineWidth)(GLfloat width);
extern void (GL_FUNCPTR* _ptr_glPointSize)(GLfloat size);
extern void (GL_FUNCPTR* _ptr_glPolygonMode)(GLenum face, GLenum mode);
extern void (GL_FUNCPTR* _ptr_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void (GL_FUNCPTR* _ptr_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
extern void (GL_FUNCPTR* _ptr_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (GL_FUNCPTR* _ptr_glTexParameteri)(GLenum target, GLenum pname, GLint param);
extern void (GL_FUNCPTR* _ptr_glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
extern void (GL_FUNCPTR* _ptr_glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR* _ptr_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR* _ptr_glDrawBuffer)(GLenum buf);
extern void (GL_FUNCPTR* _ptr_glClear)(GLbitfield mask);
extern void (GL_FUNCPTR* _ptr_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (GL_FUNCPTR* _ptr_glClearStencil)(GLint s);
extern void (GL_FUNCPTR* _ptr_glClearDepth)(GLclampd depth);
extern void (GL_FUNCPTR* _ptr_glStencilMask)(GLuint mask);
extern void (GL_FUNCPTR* _ptr_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void (GL_FUNCPTR* _ptr_glDepthMask)(GLboolean flag);
extern void (GL_FUNCPTR* _ptr_glDisable)(GLenum cap);
extern void (GL_FUNCPTR* _ptr_glEnable)(GLenum cap);
extern void (GL_FUNCPTR* _ptr_glFinish)(void);
extern void (GL_FUNCPTR* _ptr_glFlush)(void);
extern void (GL_FUNCPTR* _ptr_glBlendFunc)(GLenum sfactor, GLenum dfactor);
extern void (GL_FUNCPTR* _ptr_glLogicOp)(GLenum opcode);
extern void (GL_FUNCPTR* _ptr_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
extern void (GL_FUNCPTR* _ptr_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
extern void (GL_FUNCPTR* _ptr_glDepthFunc)(GLenum func);
extern void (GL_FUNCPTR* _ptr_glPixelStoref)(GLenum pname, GLfloat param);
extern void (GL_FUNCPTR* _ptr_glPixelStorei)(GLenum pname, GLint param);
extern void (GL_FUNCPTR* _ptr_glReadBuffer)(GLenum src);
extern void (GL_FUNCPTR* _ptr_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
extern void (GL_FUNCPTR* _ptr_glGetBooleanv)(GLenum pname, GLboolean *data);
extern void (GL_FUNCPTR* _ptr_glGetDoublev)(GLenum pname, GLdouble *data);
extern GLenum(GL_FUNCPTR* _ptr_glGetError)(void);
extern void (GL_FUNCPTR* _ptr_glGetFloatv)(GLenum pname, GLfloat *data);
extern void (GL_FUNCPTR* _ptr_glGetIntegerv)(GLenum pname, GLint *data);
extern const GLubyte *(GL_FUNCPTR* _ptr_glGetString)(GLenum name);
extern void (GL_FUNCPTR* _ptr_glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
extern void (GL_FUNCPTR* _ptr_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (GL_FUNCPTR* _ptr_glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (GL_FUNCPTR* _ptr_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void (GL_FUNCPTR* _ptr_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
extern GLboolean(GL_FUNCPTR* _ptr_glIsEnabled)(GLenum cap);
extern void (GL_FUNCPTR* _ptr_glDepthRange)(GLdouble _near, GLdouble _far);
extern void (GL_FUNCPTR* _ptr_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

inline void (GL_FUNCPTR glCullFace)(GLenum mode){
	_ptr_glCullFace(mode);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glFrontFace)(GLenum mode){
	_ptr_glFrontFace(mode);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glHint)(GLenum target, GLenum mode){
	_ptr_glHint(target, mode);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glLineWidth)(GLfloat width){
	_ptr_glLineWidth(width);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glPointSize)(GLfloat size){
	_ptr_glPointSize(size);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glPolygonMode)(GLenum face, GLenum mode){
	_ptr_glPolygonMode(face, mode);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glScissor)(GLint x, GLint y, GLsizei width, GLsizei height){
	_ptr_glScissor(x, y, width, height);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexParameterf)(GLenum target, GLenum pname, GLfloat param){
	_ptr_glTexParameterf(target, pname, param);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params){
	_ptr_glTexParameterfv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexParameteri)(GLenum target, GLenum pname, GLint param){
	_ptr_glTexParameteri(target, pname, param);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexParameteriv)(GLenum target, GLenum pname, const GLint *params){
	_ptr_glTexParameteriv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glDrawBuffer)(GLenum buf){
	_ptr_glDrawBuffer(buf);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glClear)(GLbitfield mask){
	_ptr_glClear(mask);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	_ptr_glClearColor(red, green, blue, alpha);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glClearStencil)(GLint s){
	_ptr_glClearStencil(s);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glClearDepth)(GLclampd depth){
	_ptr_glClearDepth(depth);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glStencilMask)(GLuint mask){
	_ptr_glStencilMask(mask);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha){
	_ptr_glColorMask(red, green, blue, alpha);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glDepthMask)(GLboolean flag){
	_ptr_glDepthMask(flag);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glDisable)(GLenum cap){
	_ptr_glDisable(cap);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glEnable)(GLenum cap){
	_ptr_glEnable(cap);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glFinish)(void){
	_ptr_glFinish();
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glFlush)(void){
	_ptr_glFlush();
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glBlendFunc)(GLenum sfactor, GLenum dfactor){
	_ptr_glBlendFunc(sfactor, dfactor);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glLogicOp)(GLenum opcode){
	_ptr_glLogicOp(opcode);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glStencilFunc)(GLenum func, GLint ref, GLuint mask){
	_ptr_glStencilFunc(func, ref, mask);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass){
	_ptr_glStencilOp(fail, zfail, zpass);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glDepthFunc)(GLenum func){
	_ptr_glDepthFunc(func);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glPixelStoref)(GLenum pname, GLfloat param){
	_ptr_glPixelStoref(pname, param);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glPixelStorei)(GLenum pname, GLint param){
	_ptr_glPixelStorei(pname, param);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glReadBuffer)(GLenum src){
	_ptr_glReadBuffer(src);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels){
	_ptr_glReadPixels(x, y, width, height, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetBooleanv)(GLenum pname, GLboolean *data){
	_ptr_glGetBooleanv(pname, data);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetDoublev)(GLenum pname, GLdouble *data){
	_ptr_glGetDoublev(pname, data);
	OPENGL_DEBUG_ERROR_LOG
}
inline GLenum(GL_FUNCPTR glGetError)(void){ 
	GLenum error = _ptr_glGetError();
	return error;
}
inline void (GL_FUNCPTR glGetFloatv)(GLenum pname, GLfloat *data){
	_ptr_glGetFloatv(pname, data);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetIntegerv)(GLenum pname, GLint *data){
	_ptr_glGetIntegerv(pname, data);
	OPENGL_DEBUG_ERROR_LOG
}
inline const GLubyte *(GL_FUNCPTR glGetString)(GLenum name){ 
	const GLubyte* str = _ptr_glGetString(name);
	OPENGL_DEBUG_ERROR_LOG
	return(str);
}
inline void (GL_FUNCPTR glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels){
	_ptr_glGetTexImage(target, level, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params){
	_ptr_glGetTexParameterfv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params){
	_ptr_glGetTexParameteriv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params){
	_ptr_glGetTexLevelParameterfv(target, level, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params){
	_ptr_glGetTexLevelParameteriv(target, level, pname, params);
	OPENGL_DEBUG_ERROR_LOG
}
inline GLboolean(GL_FUNCPTR glIsEnabled)(GLenum cap){
	GLboolean b = _ptr_glIsEnabled(cap);
	OPENGL_DEBUG_ERROR_LOG
	return(b);
}
inline void (GL_FUNCPTR glDepthRange)(GLdouble _near, GLdouble _far){
	_ptr_glDepthRange(_near, _far);
	OPENGL_DEBUG_ERROR_LOG
}
inline void (GL_FUNCPTR glViewport)(GLint x, GLint y, GLsizei width, GLsizei height){
	_ptr_glViewport(x, y, width, height);
	OPENGL_DEBUG_ERROR_LOG
}

//OpenGL1.1
//core

extern void (GL_FUNCPTR*_ptr_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
extern void (GL_FUNCPTR*_ptr_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
extern void (GL_FUNCPTR*_ptr_glGetPointerv)(GLenum pname, void **params);
extern void (GL_FUNCPTR*_ptr_glPolygonOffset)(GLfloat factor, GLfloat units);
extern void (GL_FUNCPTR*_ptr_glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void (GL_FUNCPTR*_ptr_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void (GL_FUNCPTR*_ptr_glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (GL_FUNCPTR*_ptr_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (GL_FUNCPTR*_ptr_glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR*_ptr_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR*_ptr_glBindTexture)(GLenum target, GLuint texture);
extern void (GL_FUNCPTR*_ptr_glDeleteTextures)(GLsizei n, const GLuint *textures);
extern void (GL_FUNCPTR*_ptr_glGenTextures)(GLsizei n, GLuint *textures);
extern GLboolean(GL_FUNCPTR*_ptr_glIsTexture)(GLuint texture);

inline void GL_FUNCPTR glDrawArrays(GLenum mode, GLint first, GLsizei count){
	_ptr_glDrawArrays(mode, first, count);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices){
	_ptr_glDrawElements(mode, count, type, indices);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glGetPointerv(GLenum pname, void **params){
	_ptr_glGetPointerv(pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glPolygonOffset(GLfloat factor, GLfloat units){
	_ptr_glPolygonOffset(factor, units);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border){
	_ptr_glCopyTexImage1D(target, level, internalformat, x, y, width, border);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border){
	_ptr_glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width){
	_ptr_glCopyTexSubImage1D(target, level, xoffset, x, y, width);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	_ptr_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glBindTexture(GLenum target, GLuint texture){
	_ptr_glBindTexture(target, texture);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glDeleteTextures(GLsizei n, const GLuint *textures){
	_ptr_glDeleteTextures(n, textures);
	OPENGL_DEBUG_ERROR_LOG
};
inline void GL_FUNCPTR glGenTextures(GLsizei n, GLuint *textures){
	_ptr_glGenTextures(n, textures);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean GL_FUNCPTR glIsTexture(GLuint texture){
	GLboolean b = _ptr_glIsTexture(texture);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};

//compatibility

extern void(GL_FUNCPTR * _ptr_glMatrixMode)(GLenum mode);
extern void(GL_FUNCPTR * _ptr_glLoadIdentity)(void);
extern void(GL_FUNCPTR * _ptr_glLoadMatrixf)(const GLfloat* m);
extern void(GL_FUNCPTR * _ptr_glBegin)(GLenum);
extern void(GL_FUNCPTR * _ptr_glEnd)(void);
extern void(GL_FUNCPTR * _ptr_glColor3f)(GLfloat, GLfloat, GLfloat);
extern void(GL_FUNCPTR * _ptr_glVertex2f)(GLfloat, GLfloat);
extern void(GL_FUNCPTR * _ptr_glVertex2i)(GLint, GLint);

inline void GL_FUNCPTR glMatrixMode (GLenum mode){
    _ptr_glMatrixMode(mode);
    OPENGL_DEBUG_ERROR_LOG
}
inline void GL_FUNCPTR glLoadIdentity (void){
    _ptr_glLoadIdentity();
    OPENGL_DEBUG_ERROR_LOG;
}
inline void GL_FUNCPTR glLoadMatrixf (const GLfloat* m){
    _ptr_glLoadMatrixf(m);
    OPENGL_DEBUG_ERROR_LOG;
}
inline void GL_FUNCPTR glBegin (GLenum mode){
	_ptr_glBegin(mode);
}
inline void GL_FUNCPTR glEnd (void){
	_ptr_glEnd();
	OPENGL_DEBUG_ERROR_LOG
}
inline void GL_FUNCPTR glColor3f (GLfloat red, GLfloat green, GLfloat blue){
	_ptr_glColor3f(red, green, blue);
}
inline void GL_FUNCPTR glVertex2f (GLfloat x, GLfloat y){
	_ptr_glVertex2f(x, y);
}
inline void GL_FUNCPTR glVertex2i(GLint x, GLint y) {
	_ptr_glVertex2i(x, y);
}

//OpenGL1.2
//core

extern void (GL_FUNCPTR * _ptr_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
extern void (GL_FUNCPTR * _ptr_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR * _ptr_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
extern void (GL_FUNCPTR * _ptr_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

inline void (GL_FUNCPTR glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices){
	_ptr_glDrawRangeElements(mode, start, end, count, type, indices);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels){
	_ptr_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height){
	_ptr_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL1.3
//core

extern void (GL_FUNCPTR * _ptr_glActiveTexture)(GLenum texture);
extern void (GL_FUNCPTR * _ptr_glSampleCoverage)(GLfloat value, GLboolean invert);
extern void (GL_FUNCPTR * _ptr_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
extern void (GL_FUNCPTR * _ptr_glGetCompressedTexImage)(GLenum target, GLint level, void *img);

inline void (GL_FUNCPTR glActiveTexture)(GLenum texture){
	_ptr_glActiveTexture(texture);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSampleCoverage)(GLfloat value, GLboolean invert){
	_ptr_glSampleCoverage(value, invert);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data){
	_ptr_glCompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetCompressedTexImage)(GLenum target, GLint level, void *img){
	_ptr_glGetCompressedTexImage(target, level, img);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL1.4
//core

extern void (GL_FUNCPTR * _ptr_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern void (GL_FUNCPTR * _ptr_glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
extern void (GL_FUNCPTR * _ptr_glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
extern void (GL_FUNCPTR * _ptr_glPointParameterf)(GLenum pname, GLfloat param);
extern void (GL_FUNCPTR * _ptr_glPointParameterfv)(GLenum pname, const GLfloat *params);
extern void (GL_FUNCPTR * _ptr_glPointParameteri)(GLenum pname, GLint param);
extern void (GL_FUNCPTR * _ptr_glPointParameteriv)(GLenum pname, const GLint *params);
extern void (GL_FUNCPTR * _ptr_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (GL_FUNCPTR * _ptr_glBlendEquation)(GLenum mode);

inline void (GL_FUNCPTR glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha){
	_ptr_glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount){
	_ptr_glMultiDrawArrays(mode, first, count, drawcount);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount){
	_ptr_glMultiDrawElements(mode, count, type, indices, drawcount);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glPointParameterf)(GLenum pname, GLfloat param){
	_ptr_glPointParameterf(pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glPointParameterfv)(GLenum pname, const GLfloat *params){
	_ptr_glPointParameterfv(pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glPointParameteri)(GLenum pname, GLint param){
	_ptr_glPointParameteri(pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glPointParameteriv)(GLenum pname, const GLint *params){
	_ptr_glPointParameteriv(pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
	_ptr_glBlendColor(red, green, blue, alpha);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBlendEquation)(GLenum mode){
	_ptr_glBlendEquation(mode);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL1.5
//core

extern void (GL_FUNCPTR * _ptr_glGenQueries)(GLsizei n, GLuint *ids);
extern void (GL_FUNCPTR * _ptr_glDeleteQueries)(GLsizei n, const GLuint *ids);
extern GLboolean(GL_FUNCPTR * _ptr_glIsQuery)(GLuint id);
extern void (GL_FUNCPTR * _ptr_glBeginQuery)(GLenum target, GLuint id);
extern void (GL_FUNCPTR * _ptr_glEndQuery)(GLenum target);
extern void (GL_FUNCPTR * _ptr_glGetQueryiv)(GLenum target, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
extern void (GL_FUNCPTR * _ptr_glBindBuffer)(GLenum target, GLuint buffer);
extern void (GL_FUNCPTR * _ptr_glDeleteBuffers)(GLsizei n, const GLuint *buffers);
extern void (GL_FUNCPTR * _ptr_glGenBuffers)(GLsizei n, GLuint *buffers);
extern GLboolean(GL_FUNCPTR * _ptr_glIsBuffer)(GLuint buffer);
extern void (GL_FUNCPTR * _ptr_glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern void (GL_FUNCPTR * _ptr_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
extern void (GL_FUNCPTR * _ptr_glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, void *data);
extern void *(GL_FUNCPTR * _ptr_glMapBuffer)(GLenum target, GLenum access);
extern GLboolean(GL_FUNCPTR * _ptr_glUnmapBuffer)(GLenum target);
extern void (GL_FUNCPTR * _ptr_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetBufferPointerv)(GLenum target, GLenum pname, void **params);

inline void (GL_FUNCPTR glGenQueries)(GLsizei n, GLuint *ids){
	_ptr_glGenQueries(n, ids);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteQueries)(GLsizei n, const GLuint *ids){
	_ptr_glDeleteQueries(n, ids);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsQuery)(GLuint id){
	GLboolean b = _ptr_glIsQuery(id);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBeginQuery)(GLenum target, GLuint id){
	_ptr_glBeginQuery(target, id);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glEndQuery)(GLenum target){
	_ptr_glEndQuery(target);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetQueryiv)(GLenum target, GLenum pname, GLint *params){
	_ptr_glGetQueryiv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params){
	_ptr_glGetQueryObjectiv(id, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params){
	_ptr_glGetQueryObjectuiv(id, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindBuffer)(GLenum target, GLuint buffer){
	_ptr_glBindBuffer(target, buffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteBuffers)(GLsizei n, const GLuint *buffers){
	_ptr_glDeleteBuffers(n, buffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGenBuffers)(GLsizei n, GLuint *buffers){
	_ptr_glGenBuffers(n, buffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsBuffer)(GLuint buffer){
	GLboolean b = _ptr_glIsBuffer(buffer);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage){
	_ptr_glBufferData(target, size, data, usage);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data){
	_ptr_glBufferSubData(target, offset, size, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, void *data){
	_ptr_glGetBufferSubData(target, offset, size, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void *(GL_FUNCPTR glMapBuffer)(GLenum target, GLenum access){
	void * ptr = _ptr_glMapBuffer(target, access);
	OPENGL_DEBUG_ERROR_LOG;
	return(ptr);
};
inline GLboolean(GL_FUNCPTR glUnmapBuffer)(GLenum target){
	GLboolean b = _ptr_glUnmapBuffer(target);
	OPENGL_DEBUG_ERROR_LOG
	return(b);
};
inline void (GL_FUNCPTR glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params){
	_ptr_glGetBufferParameteriv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetBufferPointerv)(GLenum target, GLenum pname, void **params){
	_ptr_glGetBufferPointerv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL2.0
//core
extern void (GL_FUNCPTR * _ptr_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
extern void (GL_FUNCPTR * _ptr_glDrawBuffers)(GLsizei n, const GLenum *bufs);
extern void (GL_FUNCPTR * _ptr_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void (GL_FUNCPTR * _ptr_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
extern void (GL_FUNCPTR * _ptr_glStencilMaskSeparate)(GLenum face, GLuint mask);
extern void (GL_FUNCPTR * _ptr_glAttachShader)(GLuint program, GLuint shader);
extern void (GL_FUNCPTR * _ptr_glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
extern void (GL_FUNCPTR * _ptr_glCompileShader)(GLuint shader);
extern GLuint(GL_FUNCPTR * _ptr_glCreateProgram)(void);
extern GLuint(GL_FUNCPTR * _ptr_glCreateShader)(GLenum type);
extern void (GL_FUNCPTR * _ptr_glDeleteProgram)(GLuint program);
extern void (GL_FUNCPTR * _ptr_glDeleteShader)(GLuint shader);
extern void (GL_FUNCPTR * _ptr_glDetachShader)(GLuint program, GLuint shader);
extern void (GL_FUNCPTR * _ptr_glDisableVertexAttribArray)(GLuint index);
extern void (GL_FUNCPTR * _ptr_glEnableVertexAttribArray)(GLuint index);
extern void (GL_FUNCPTR * _ptr_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void (GL_FUNCPTR * _ptr_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void (GL_FUNCPTR * _ptr_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
extern GLint(GL_FUNCPTR * _ptr_glGetAttribLocation)(GLuint program, const GLchar *name);
extern void (GL_FUNCPTR * _ptr_glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (GL_FUNCPTR * _ptr_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (GL_FUNCPTR * _ptr_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern GLint(GL_FUNCPTR * _ptr_glGetUniformLocation)(GLuint program, const GLchar *name);
extern void (GL_FUNCPTR * _ptr_glGetUniformfv)(GLuint program, GLint location, GLfloat *params);
extern void (GL_FUNCPTR * _ptr_glGetUniformiv)(GLuint program, GLint location, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer);
extern GLboolean(GL_FUNCPTR * _ptr_glIsProgram)(GLuint program);
extern GLboolean(GL_FUNCPTR * _ptr_glIsShader)(GLuint shader);
extern void (GL_FUNCPTR * _ptr_glLinkProgram)(GLuint program);
extern void (GL_FUNCPTR * _ptr_glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern void (GL_FUNCPTR * _ptr_glUseProgram)(GLuint program);
extern void (GL_FUNCPTR * _ptr_glUniform1f)(GLint location, GLfloat v0);
extern void (GL_FUNCPTR * _ptr_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
extern void (GL_FUNCPTR * _ptr_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (GL_FUNCPTR * _ptr_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (GL_FUNCPTR * _ptr_glUniform1i)(GLint location, GLint v0);
extern void (GL_FUNCPTR * _ptr_glUniform2i)(GLint location, GLint v0, GLint v1);
extern void (GL_FUNCPTR * _ptr_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
extern void (GL_FUNCPTR * _ptr_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (GL_FUNCPTR * _ptr_glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniform1iv)(GLint location, GLsizei count, const GLint *value);
extern void (GL_FUNCPTR * _ptr_glUniform2iv)(GLint location, GLsizei count, const GLint *value);
extern void (GL_FUNCPTR * _ptr_glUniform3iv)(GLint location, GLsizei count, const GLint *value);
extern void (GL_FUNCPTR * _ptr_glUniform4iv)(GLint location, GLsizei count, const GLint *value);
extern void (GL_FUNCPTR * _ptr_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glValidateProgram)(GLuint program);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1d)(GLuint index, GLdouble x);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1dv)(GLuint index, const GLdouble *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1f)(GLuint index, GLfloat x);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1fv)(GLuint index, const GLfloat *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1s)(GLuint index, GLshort x);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib1sv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2dv)(GLuint index, const GLdouble *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2fv)(GLuint index, const GLfloat *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib2sv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3dv)(GLuint index, const GLdouble *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3fv)(GLuint index, const GLfloat *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib3sv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Niv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nsv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4Nusv)(GLuint index, const GLushort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4bv)(GLuint index, const GLbyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4dv)(GLuint index, const GLdouble *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4fv)(GLuint index, const GLfloat *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4iv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4sv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4ubv)(GLuint index, const GLubyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4uiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttrib4usv)(GLuint index, const GLushort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

inline void (GL_FUNCPTR glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha){
	_ptr_glBlendEquationSeparate(modeRGB, modeAlpha);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDrawBuffers)(GLsizei n, const GLenum *bufs){
	_ptr_glDrawBuffers(n, bufs);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass){
	_ptr_glStencilOpSeparate(face, sfail, dpfail, dppass);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask){
	_ptr_glStencilFuncSeparate(face, func, ref, mask);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glStencilMaskSeparate)(GLenum face, GLuint mask){
	_ptr_glStencilMaskSeparate(face, mask);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glAttachShader)(GLuint program, GLuint shader){
	_ptr_glAttachShader(program, shader);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name){
	_ptr_glBindAttribLocation(program, index, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCompileShader)(GLuint shader){
	_ptr_glCompileShader(shader);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLuint(GL_FUNCPTR glCreateProgram)(void){
	GLuint program = _ptr_glCreateProgram();
	OPENGL_DEBUG_ERROR_LOG;
	return(program);
};
inline GLuint(GL_FUNCPTR glCreateShader)(GLenum type){
	GLuint shader = _ptr_glCreateShader(type);
	OPENGL_DEBUG_ERROR_LOG;
	return(shader);
};
inline void (GL_FUNCPTR glDeleteProgram)(GLuint program){
	_ptr_glDeleteProgram(program);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteShader)(GLuint shader){
	_ptr_glDeleteShader(shader);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDetachShader)(GLuint program, GLuint shader){
	_ptr_glDetachShader(program, shader);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDisableVertexAttribArray)(GLuint index){
	_ptr_glDisableVertexAttribArray(index);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glEnableVertexAttribArray)(GLuint index){
	_ptr_glEnableVertexAttribArray(index);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){
	_ptr_glGetActiveAttrib(program, index, bufSize, length, size, type, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){
	_ptr_glGetActiveUniform(program, index, bufSize, length, size, type, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders){
	_ptr_glGetAttachedShaders(program, maxCount, count, shaders);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLint(GL_FUNCPTR glGetAttribLocation)(GLuint program, const GLchar *name){
	GLint loc = _ptr_glGetAttribLocation(program, name);
	OPENGL_DEBUG_ERROR_LOG;
	return loc;
};
inline void (GL_FUNCPTR glGetProgramiv)(GLuint program, GLenum pname, GLint *params){
	_ptr_glGetProgramiv(program, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	_ptr_glGetProgramInfoLog(program, bufSize, length, infoLog);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetShaderiv)(GLuint shader, GLenum pname, GLint *params){
	_ptr_glGetShaderiv(shader, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog){
	_ptr_glGetShaderInfoLog(shader, bufSize, length, infoLog);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source){
	_ptr_glGetShaderSource(shader, bufSize, length, source);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLint(GL_FUNCPTR glGetUniformLocation)(GLuint program, const GLchar *name){
	GLint loc = _ptr_glGetUniformLocation(program, name);
	OPENGL_DEBUG_ERROR_LOG;
	return(loc);
};
inline void (GL_FUNCPTR glGetUniformfv)(GLuint program, GLint location, GLfloat *params){
	_ptr_glGetUniformfv(program, location, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetUniformiv)(GLuint program, GLint location, GLint *params){
	_ptr_glGetUniformiv(program, location, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params){
	_ptr_glGetVertexAttribdv(index, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params){
	_ptr_glGetVertexAttribfv(index, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params){
	_ptr_glGetVertexAttribiv(index, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer){
	_ptr_glGetVertexAttribPointerv(index, pname, pointer);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsProgram)(GLuint program){
	GLboolean b = _ptr_glIsProgram(program);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline GLboolean(GL_FUNCPTR glIsShader)(GLuint shader){
	GLboolean b = _ptr_glIsShader(shader);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glLinkProgram)(GLuint program){
	_ptr_glLinkProgram(program);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length){
	_ptr_glShaderSource(shader, count, string, length);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUseProgram)(GLuint program){
	_ptr_glUseProgram(program);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform1f)(GLint location, GLfloat v0){
	_ptr_glUniform1f(location, v0);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2f)(GLint location, GLfloat v0, GLfloat v1){
	_ptr_glUniform2f(location, v0, v1);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
	_ptr_glUniform3f(location, v0, v1, v2);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	_ptr_glUniform4f(location, v0, v1, v2, v3);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform1i)(GLint location, GLint v0){
	_ptr_glUniform1i(location, v0);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2i)(GLint location, GLint v0, GLint v1){
	_ptr_glUniform2i(location, v0, v1);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2){
	_ptr_glUniform3i(location, v0, v1, v2);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	_ptr_glUniform4i(location, v0, v1, v2, v3);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform1fv)(GLint location, GLsizei count, const GLfloat *value){
	_ptr_glUniform1fv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2fv)(GLint location, GLsizei count, const GLfloat *value){
	_ptr_glUniform2fv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3fv)(GLint location, GLsizei count, const GLfloat *value){
	_ptr_glUniform3fv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4fv)(GLint location, GLsizei count, const GLfloat *value){
	_ptr_glUniform4fv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform1iv)(GLint location, GLsizei count, const GLint *value){
	_ptr_glUniform1iv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2iv)(GLint location, GLsizei count, const GLint *value){
	_ptr_glUniform2iv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3iv)(GLint location, GLsizei count, const GLint *value){
	_ptr_glUniform3iv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4iv)(GLint location, GLsizei count, const GLint *value){
	_ptr_glUniform4iv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	_ptr_glUniformMatrix2fv(location, count, transpose, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	_ptr_glUniformMatrix3fv(location, count, transpose, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
	_ptr_glUniformMatrix4fv(location, count, transpose, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glValidateProgram)(GLuint program){
	_ptr_glValidateProgram(program);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1d)(GLuint index, GLdouble x){
	_ptr_glVertexAttrib1d(index, x);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1dv)(GLuint index, const GLdouble *v){
	_ptr_glVertexAttrib1dv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1f)(GLuint index, GLfloat x){
	_ptr_glVertexAttrib1f(index, x);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1fv)(GLuint index, const GLfloat *v){
	_ptr_glVertexAttrib1fv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1s)(GLuint index, GLshort x){
	_ptr_glVertexAttrib1s(index, x);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib1sv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttrib1sv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y){
	_ptr_glVertexAttrib2d(index, x, y);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2dv)(GLuint index, const GLdouble *v){
	_ptr_glVertexAttrib2dv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y){
	_ptr_glVertexAttrib2f(index, x, y);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2fv)(GLuint index, const GLfloat *v){
	_ptr_glVertexAttrib2fv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2s)(GLuint index, GLshort x, GLshort y){
	_ptr_glVertexAttrib2s(index, x, y);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib2sv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttrib2sv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z){
	_ptr_glVertexAttrib3d(index, x, y, z);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3dv)(GLuint index, const GLdouble *v){
	_ptr_glVertexAttrib3dv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z){
	_ptr_glVertexAttrib3f(index, x, y, z);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3fv)(GLuint index, const GLfloat *v){
	_ptr_glVertexAttrib3fv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z){
	_ptr_glVertexAttrib3s(index, x, y, z);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib3sv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttrib3sv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nbv)(GLuint index, const GLbyte *v){
	_ptr_glVertexAttrib4Nbv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Niv)(GLuint index, const GLint *v){
	_ptr_glVertexAttrib4Niv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nsv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttrib4Nsv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w){
	_ptr_glVertexAttrib4Nub(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nubv)(GLuint index, const GLubyte *v){
	_ptr_glVertexAttrib4Nubv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nuiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttrib4Nuiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4Nusv)(GLuint index, const GLushort *v){
	_ptr_glVertexAttrib4Nusv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4bv)(GLuint index, const GLbyte *v){
	_ptr_glVertexAttrib4bv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	_ptr_glVertexAttrib4d(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4dv)(GLuint index, const GLdouble *v){
	_ptr_glVertexAttrib4dv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	_ptr_glVertexAttrib4f(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4fv)(GLuint index, const GLfloat *v){
	_ptr_glVertexAttrib4fv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4iv)(GLuint index, const GLint *v){
	_ptr_glVertexAttrib4iv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w){
	_ptr_glVertexAttrib4s(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4sv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttrib4sv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4ubv)(GLuint index, const GLubyte *v){
	_ptr_glVertexAttrib4ubv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4uiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttrib4uiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttrib4usv)(GLuint index, const GLushort *v){
	_ptr_glVertexAttrib4usv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer){
	_ptr_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	OPENGL_DEBUG_ERROR_LOG
};
//OpenGL2.1
//core

extern void (GL_FUNCPTR*_ptr_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR*_ptr_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR*_ptr_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR*_ptr_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR*_ptr_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GL_FUNCPTR*_ptr_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// TODO implement these!
inline void (GL_FUNCPTR glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
inline void (GL_FUNCPTR glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
inline void (GL_FUNCPTR glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
inline void (GL_FUNCPTR glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
inline void (GL_FUNCPTR glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
inline void (GL_FUNCPTR glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

//OpenGL3.0
//core

extern void (GL_FUNCPTR * _ptr_glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern void (GL_FUNCPTR * _ptr_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
extern void (GL_FUNCPTR * _ptr_glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
extern void (GL_FUNCPTR * _ptr_glEnablei)(GLenum target, GLuint index);
extern void (GL_FUNCPTR * _ptr_glDisablei)(GLenum target, GLuint index);
extern GLboolean(GL_FUNCPTR * _ptr_glIsEnabledi)(GLenum target, GLuint index);
extern void (GL_FUNCPTR * _ptr_glBeginTransformFeedback)(GLenum primitiveMode);
extern void (GL_FUNCPTR * _ptr_glEndTransformFeedback)(void);
extern void (GL_FUNCPTR * _ptr_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void (GL_FUNCPTR * _ptr_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
extern void (GL_FUNCPTR * _ptr_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
extern void (GL_FUNCPTR * _ptr_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void (GL_FUNCPTR * _ptr_glClampColor)(GLenum target, GLenum clamp);
extern void (GL_FUNCPTR * _ptr_glBeginConditionalRender)(GLuint id, GLenum mode);
extern void (GL_FUNCPTR * _ptr_glEndConditionalRender)(void);
extern void (GL_FUNCPTR * _ptr_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI1i)(GLuint index, GLint x);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI2i)(GLuint index, GLint x, GLint y);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI1ui)(GLuint index, GLuint x);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI1iv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI2iv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI3iv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4iv)(GLuint index, const GLint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI1uiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI2uiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI3uiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4uiv)(GLuint index, const GLuint *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4bv)(GLuint index, const GLbyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4sv)(GLuint index, const GLshort *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4ubv)(GLuint index, const GLubyte *v);
extern void (GL_FUNCPTR * _ptr_glVertexAttribI4usv)(GLuint index, const GLushort *v);
extern void (GL_FUNCPTR * _ptr_glGetUniformuiv)(GLuint program, GLint location, GLuint *params);
extern void (GL_FUNCPTR * _ptr_glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
extern GLint(GL_FUNCPTR * _ptr_glGetFragDataLocation)(GLuint program, const GLchar *name);
extern void (GL_FUNCPTR * _ptr_glUniform1ui)(GLint location, GLuint v0);
extern void (GL_FUNCPTR * _ptr_glUniform2ui)(GLint location, GLuint v0, GLuint v1);
extern void (GL_FUNCPTR * _ptr_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern void (GL_FUNCPTR * _ptr_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern void (GL_FUNCPTR * _ptr_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
extern void (GL_FUNCPTR * _ptr_glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
extern void (GL_FUNCPTR * _ptr_glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
extern void (GL_FUNCPTR * _ptr_glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
extern void (GL_FUNCPTR * _ptr_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
extern void (GL_FUNCPTR * _ptr_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
extern const GLubyte *(GL_FUNCPTR * _ptr_glGetStringi)(GLenum name, GLuint index);
extern GLboolean(GL_FUNCPTR * _ptr_glIsRenderbuffer)(GLuint renderbuffer);
extern void (GL_FUNCPTR * _ptr_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
extern void (GL_FUNCPTR * _ptr_glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
extern void (GL_FUNCPTR * _ptr_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
extern void (GL_FUNCPTR * _ptr_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void (GL_FUNCPTR * _ptr_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
extern GLboolean(GL_FUNCPTR * _ptr_glIsFramebuffer)(GLuint framebuffer);
extern void (GL_FUNCPTR * _ptr_glBindFramebuffer)(GLenum target, GLuint framebuffer);
extern void (GL_FUNCPTR * _ptr_glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
extern void (GL_FUNCPTR * _ptr_glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
extern GLenum(GL_FUNCPTR * _ptr_glCheckFramebufferStatus)(GLenum target);
extern void (GL_FUNCPTR * _ptr_glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (GL_FUNCPTR * _ptr_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (GL_FUNCPTR * _ptr_glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern void (GL_FUNCPTR * _ptr_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void (GL_FUNCPTR * _ptr_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGenerateMipmap)(GLenum target);
extern void (GL_FUNCPTR * _ptr_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern void (GL_FUNCPTR * _ptr_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void (GL_FUNCPTR * _ptr_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern void *(GL_FUNCPTR * _ptr_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern void (GL_FUNCPTR * _ptr_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
extern void (GL_FUNCPTR * _ptr_glBindVertexArray)(GLuint _array);
extern void (GL_FUNCPTR * _ptr_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
extern void (GL_FUNCPTR * _ptr_glGenVertexArrays)(GLsizei n, GLuint *arrays);
extern GLboolean(GL_FUNCPTR * _ptr_glIsVertexArray)(GLuint _array);

inline void (GL_FUNCPTR glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
	_ptr_glColorMaski(index, r, g, b, a);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data){
	_ptr_glGetBooleani_v(target, index, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetIntegeri_v)(GLenum target, GLuint index, GLint *data){
	_ptr_glGetIntegeri_v(target, index, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glEnablei)(GLenum target, GLuint index){
	_ptr_glEnablei(target, index);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDisablei)(GLenum target, GLuint index){
	_ptr_glDisablei(target, index);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsEnabledi)(GLenum target, GLuint index){
	GLboolean b = _ptr_glIsEnabledi(target, index);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBeginTransformFeedback)(GLenum primitiveMode){
	_ptr_glBeginTransformFeedback(primitiveMode);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glEndTransformFeedback)(void){
	_ptr_glEndTransformFeedback();
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size){
	_ptr_glBindBufferRange(target, index, buffer, offset, size);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindBufferBase)(GLenum target, GLuint index, GLuint buffer){
	_ptr_glBindBufferBase(target, index, buffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode){
	_ptr_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name){
	_ptr_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glClampColor)(GLenum target, GLenum clamp){
	_ptr_glClampColor(target, clamp);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBeginConditionalRender)(GLuint id, GLenum mode){
	_ptr_glBeginConditionalRender(id, mode);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glEndConditionalRender)(void){
	_ptr_glEndConditionalRender();
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer){
	_ptr_glVertexAttribIPointer(index, size, type, stride, pointer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params){
	_ptr_glGetVertexAttribIiv(index, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params){
	_ptr_glGetVertexAttribIuiv(index, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI1i)(GLuint index, GLint x){
	_ptr_glVertexAttribI1i(index, x);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI2i)(GLuint index, GLint x, GLint y){
	_ptr_glVertexAttribI2i(index, x, y);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z){
	_ptr_glVertexAttribI3i(index, x, y, z);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w){
	_ptr_glVertexAttribI4i(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI1ui)(GLuint index, GLuint x){
	_ptr_glVertexAttribI1ui(index, x);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y){
	_ptr_glVertexAttribI2ui(index, x, y);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z){
	_ptr_glVertexAttribI3ui(index, x, y, z);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w){
	_ptr_glVertexAttribI4ui(index, x, y, z, w);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI1iv)(GLuint index, const GLint *v){
	_ptr_glVertexAttribI1iv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI2iv)(GLuint index, const GLint *v){
	_ptr_glVertexAttribI2iv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI3iv)(GLuint index, const GLint *v){
	_ptr_glVertexAttribI3iv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4iv)(GLuint index, const GLint *v){
	_ptr_glVertexAttribI4iv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI1uiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttribI1uiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI2uiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttribI2uiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI3uiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttribI3uiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4uiv)(GLuint index, const GLuint *v){
	_ptr_glVertexAttribI4uiv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4bv)(GLuint index, const GLbyte *v){
	_ptr_glVertexAttribI4bv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4sv)(GLuint index, const GLshort *v){
	_ptr_glVertexAttribI4sv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4ubv)(GLuint index, const GLubyte *v){
	_ptr_glVertexAttribI4ubv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribI4usv)(GLuint index, const GLushort *v){
	_ptr_glVertexAttribI4usv(index, v);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetUniformuiv)(GLuint program, GLint location, GLuint *params){
	_ptr_glGetUniformuiv(program, location, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name){
	_ptr_glBindFragDataLocation(program, color, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLint(GL_FUNCPTR glGetFragDataLocation)(GLuint program, const GLchar *name){
	GLint loc = _ptr_glGetFragDataLocation(program, name);
	OPENGL_DEBUG_ERROR_LOG
	return(loc);
};
inline void (GL_FUNCPTR glUniform1ui)(GLint location, GLuint v0){
	_ptr_glUniform1ui(location, v0);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2ui)(GLint location, GLuint v0, GLuint v1){
	_ptr_glUniform2ui(location, v0, v1);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2){
	_ptr_glUniform3ui(location, v0, v1, v2);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	_ptr_glUniform4ui(location, v0, v1, v2, v3);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform1uiv)(GLint location, GLsizei count, const GLuint *value){
	_ptr_glUniform1uiv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform2uiv)(GLint location, GLsizei count, const GLuint *value){
	_ptr_glUniform2uiv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform3uiv)(GLint location, GLsizei count, const GLuint *value){
	_ptr_glUniform3uiv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniform4uiv)(GLint location, GLsizei count, const GLuint *value){
	_ptr_glUniform4uiv(location, count, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params){
	_ptr_glTexParameterIiv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params){
	_ptr_glTexParameterIuiv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params){
	_ptr_glGetTexParameterIiv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params){
	_ptr_glGetTexParameterIuiv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value){
	_ptr_glClearBufferiv(buffer, drawbuffer, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value){
	_ptr_glClearBufferuiv(buffer, drawbuffer, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value){
	_ptr_glClearBufferfv(buffer, drawbuffer, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil){
	_ptr_glClearBufferfi(buffer, drawbuffer, depth, stencil);
	OPENGL_DEBUG_ERROR_LOG
};
inline const GLubyte *(GL_FUNCPTR glGetStringi)(GLenum name, GLuint index){
	const GLubyte* str = _ptr_glGetStringi(name, index);
	OPENGL_DEBUG_ERROR_LOG;
	return(str);
};
inline GLboolean(GL_FUNCPTR glIsRenderbuffer)(GLuint renderbuffer){
	GLboolean b = _ptr_glIsRenderbuffer(renderbuffer);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBindRenderbuffer)(GLenum target, GLuint renderbuffer){
	_ptr_glBindRenderbuffer(target, renderbuffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers){
	_ptr_glDeleteRenderbuffers(n, renderbuffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers){
	_ptr_glGenRenderbuffers(n, renderbuffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
	_ptr_glRenderbufferStorage(target, internalformat, width, height);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params){
	_ptr_glGetRenderbufferParameteriv(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsFramebuffer)(GLuint framebuffer){
	GLboolean b = _ptr_glIsFramebuffer(framebuffer);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBindFramebuffer)(GLenum target, GLuint framebuffer){
	_ptr_glBindFramebuffer(target, framebuffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers){
	_ptr_glDeleteFramebuffers(n, framebuffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGenFramebuffers)(GLsizei n, GLuint *framebuffers){
	_ptr_glGenFramebuffers(n, framebuffers);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLenum(GL_FUNCPTR glCheckFramebufferStatus)(GLenum target){
	GLenum status = _ptr_glCheckFramebufferStatus(target);
	OPENGL_DEBUG_ERROR_LOG;
	return status;
};
inline void (GL_FUNCPTR glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	_ptr_glFramebufferTexture1D(target, attachment, textarget, texture, level);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
	_ptr_glFramebufferTexture2D(target, attachment, textarget, texture, level);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){
	_ptr_glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
	_ptr_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params){
	_ptr_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGenerateMipmap)(GLenum target){
	_ptr_glGenerateMipmap(target);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){
	_ptr_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){
	_ptr_glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer){
	_ptr_glFramebufferTextureLayer(target, attachment, texture, level, layer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void *(GL_FUNCPTR glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access){
	void* ptr = _ptr_glMapBufferRange(target, offset, length, access);
	OPENGL_DEBUG_ERROR_LOG;
	return ptr;
};
inline void (GL_FUNCPTR glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length){
	_ptr_glFlushMappedBufferRange(target, offset, length);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glBindVertexArray)(GLuint _array){
	_ptr_glBindVertexArray(_array);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteVertexArrays)(GLsizei n, const GLuint *arrays){
	_ptr_glDeleteVertexArrays(n, arrays);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGenVertexArrays)(GLsizei n, GLuint *arrays){
	_ptr_glGenVertexArrays(n, arrays);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsVertexArray)(GLuint _array){
	GLboolean b = _ptr_glIsVertexArray(_array);
	OPENGL_DEBUG_ERROR_LOG;
	return b;
};

//OpenGL3.1
//core

extern void (GL_FUNCPTR * _ptr_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
extern void (GL_FUNCPTR * _ptr_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
extern void (GL_FUNCPTR * _ptr_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
extern void (GL_FUNCPTR * _ptr_glPrimitiveRestartIndex)(GLuint index);
extern void (GL_FUNCPTR * _ptr_glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern void (GL_FUNCPTR * _ptr_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
extern void (GL_FUNCPTR * _ptr_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
extern GLuint(GL_FUNCPTR * _ptr_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
extern void (GL_FUNCPTR * _ptr_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
extern void (GL_FUNCPTR * _ptr_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

inline void (GL_FUNCPTR glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount){
	_ptr_glDrawArraysInstanced(mode, first, count, instancecount);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount){
	_ptr_glDrawElementsInstanced(mode, count, type, indices, instancecount);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer){
	_ptr_glTexBuffer(target, internalformat, buffer);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glPrimitiveRestartIndex)(GLuint index){
	_ptr_glPrimitiveRestartIndex(index);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
	_ptr_glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices){
	_ptr_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params){
	_ptr_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName){
	_ptr_glGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLuint(GL_FUNCPTR glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName){
	GLuint index = _ptr_glGetUniformBlockIndex(program, uniformBlockName);
	OPENGL_DEBUG_ERROR_LOG;
	return(index);
};
inline void (GL_FUNCPTR glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params){
	_ptr_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName){
	_ptr_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding){
	_ptr_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL3.2
//core

extern void (GL_FUNCPTR * _ptr_glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern void (GL_FUNCPTR * _ptr_glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern void (GL_FUNCPTR * _ptr_glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
extern void (GL_FUNCPTR * _ptr_glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
extern void (GL_FUNCPTR * _ptr_glProvokingVertex)(GLenum mode);
extern GLsync(GL_FUNCPTR * _ptr_glFenceSync)(GLenum condition, GLbitfield flags);
extern GLboolean(GL_FUNCPTR * _ptr_glIsSync)(GLsync sync);
extern void (GL_FUNCPTR * _ptr_glDeleteSync)(GLsync sync);
extern GLenum(GL_FUNCPTR * _ptr_glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern void (GL_FUNCPTR * _ptr_glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern void (GL_FUNCPTR * _ptr_glGetInteger64v)(GLenum pname, GLint64 *data);
extern void (GL_FUNCPTR * _ptr_glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
extern void (GL_FUNCPTR * _ptr_glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data);
extern void (GL_FUNCPTR * _ptr_glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);
extern void (GL_FUNCPTR * _ptr_glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern void (GL_FUNCPTR * _ptr_glTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void (GL_FUNCPTR * _ptr_glTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void (GL_FUNCPTR * _ptr_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
extern void (GL_FUNCPTR * _ptr_glSampleMaski)(GLuint maskNumber, GLbitfield mask);

inline void (GL_FUNCPTR glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex){
	_ptr_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex){
	_ptr_glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex){
	_ptr_glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex){
	_ptr_glMultiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glProvokingVertex)(GLenum mode){
	_ptr_glProvokingVertex(mode);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLsync(GL_FUNCPTR glFenceSync)(GLenum condition, GLbitfield flags){
	GLsync sync = _ptr_glFenceSync(condition, flags);
	OPENGL_DEBUG_ERROR_LOG;
	return(sync);
};
inline GLboolean(GL_FUNCPTR glIsSync)(GLsync sync){
	GLboolean b = _ptr_glIsSync(sync);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glDeleteSync)(GLsync sync){
	_ptr_glDeleteSync(sync);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLenum(GL_FUNCPTR glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout){
	GLenum syncE = _ptr_glClientWaitSync(sync, flags, timeout);
	OPENGL_DEBUG_ERROR_LOG;
	return(syncE);
};
inline void (GL_FUNCPTR glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout){
	_ptr_glWaitSync(sync, flags, timeout);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetInteger64v)(GLenum pname, GLint64 *data){
	_ptr_glGetInteger64v(pname, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values){
	_ptr_glGetSynciv(sync, pname, bufSize, length, values);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data){
	_ptr_glGetInteger64i_v(target, index, data);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params){
	_ptr_glGetBufferParameteri64v(target, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level){
	_ptr_glFramebufferTexture(target, attachment, texture, level);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations){
	_ptr_glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations){
	_ptr_glTexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val){
	_ptr_glGetMultisamplefv(pname, index, val);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSampleMaski)(GLuint maskNumber, GLbitfield mask){
	_ptr_glSampleMaski(maskNumber, mask);
	OPENGL_DEBUG_ERROR_LOG
};

//OpenGL3.3
//core

extern void (GL_FUNCPTR * _ptr_glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
extern GLint(GL_FUNCPTR * _ptr_glGetFragDataIndex)(GLuint program, const GLchar *name);
extern void (GL_FUNCPTR * _ptr_glGenSamplers)(GLsizei count, GLuint *samplers);
extern void (GL_FUNCPTR * _ptr_glDeleteSamplers)(GLsizei count, const GLuint *samplers);
extern GLboolean(GL_FUNCPTR * _ptr_glIsSampler)(GLuint sampler);
extern void (GL_FUNCPTR * _ptr_glBindSampler)(GLuint unit, GLuint sampler);
extern void (GL_FUNCPTR * _ptr_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
extern void (GL_FUNCPTR * _ptr_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
extern void (GL_FUNCPTR * _ptr_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
extern void (GL_FUNCPTR * _ptr_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
extern void (GL_FUNCPTR * _ptr_glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
extern void (GL_FUNCPTR * _ptr_glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
extern void (GL_FUNCPTR * _ptr_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
extern void (GL_FUNCPTR * _ptr_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
extern void (GL_FUNCPTR * _ptr_glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
extern void (GL_FUNCPTR * _ptr_glQueryCounter)(GLuint id, GLenum target);
extern void (GL_FUNCPTR * _ptr_glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64 *params);
extern void (GL_FUNCPTR * _ptr_glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params);
extern void (GL_FUNCPTR * _ptr_glVertexAttribDivisor)(GLuint index, GLuint divisor);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (GL_FUNCPTR * _ptr_glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);

inline void (GL_FUNCPTR glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name){
	_ptr_glBindFragDataLocationIndexed(program, colorNumber, index, name);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLint(GL_FUNCPTR glGetFragDataIndex)(GLuint program, const GLchar *name){
	GLint index = _ptr_glGetFragDataIndex(program, name);
	OPENGL_DEBUG_ERROR_LOG;
	return(index);
};
inline void (GL_FUNCPTR glGenSamplers)(GLsizei count, GLuint *samplers){
	_ptr_glGenSamplers(count, samplers);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glDeleteSamplers)(GLsizei count, const GLuint *samplers){
	_ptr_glDeleteSamplers(count, samplers);
	OPENGL_DEBUG_ERROR_LOG
};
inline GLboolean(GL_FUNCPTR glIsSampler)(GLuint sampler){
	GLboolean b = _ptr_glIsSampler(sampler);
	OPENGL_DEBUG_ERROR_LOG;
	return(b);
};
inline void (GL_FUNCPTR glBindSampler)(GLuint unit, GLuint sampler){
	_ptr_glBindSampler(unit, sampler);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param){
	_ptr_glSamplerParameteri(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param){
	_ptr_glSamplerParameteriv(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param){
	_ptr_glSamplerParameterf(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param){
	_ptr_glSamplerParameterfv(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param){
	_ptr_glSamplerParameterIiv(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param){
	_ptr_glSamplerParameterIuiv(sampler, pname, param);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params){
	_ptr_glSamplerParameteriv(sampler, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params){
	_ptr_glGetSamplerParameterIiv(sampler, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params){
	_ptr_glGetSamplerParameterfv(sampler, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params){
	_ptr_glGetSamplerParameterIuiv(sampler, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glQueryCounter)(GLuint id, GLenum target){
	_ptr_glQueryCounter(id, target);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64 *params){
	_ptr_glGetQueryObjecti64v(id, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params){
	_ptr_glGetQueryObjectui64v(id, pname, params);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribDivisor)(GLuint index, GLuint divisor){
	_ptr_glVertexAttribDivisor(index, divisor);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	_ptr_glVertexAttribP1ui(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	_ptr_glVertexAttribP1uiv(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	_ptr_glVertexAttribP2ui(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	_ptr_glVertexAttribP2uiv(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	_ptr_glVertexAttribP3ui(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	_ptr_glVertexAttribP3uiv(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value){
	_ptr_glVertexAttribP4ui(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};
inline void (GL_FUNCPTR glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value){
	_ptr_glVertexAttribP4uiv(index, type, normalized, value);
	OPENGL_DEBUG_ERROR_LOG
};


void * GetGLProcAddress(const char *name);

template <typename Ret, typename... Args>
Ret GL_FUNCPTR CoreProfileError(Args... args){
	std::cout << "WARNING: Function not Avaliable in Core Profile" << std::endl;
	return Ret(0);
}

template <typename... Args>
void GL_FUNCPTR VoidCoreProfileError(Args... args){
	std::cout << "WARNING: Function not Avaliable in Core Profile" << std::endl;
}

static int Load_GL_Version_1_0(bool _coreProfile){
	int numFailed = 0;
	if (!_coreProfile){
        LOAD_FUNCTION_NAME(_ptr_glMatrixMode, void(GL_FUNCPTR *)(GLenum), "glMatrixMode");
        LOAD_FUNCTION_NAME(_ptr_glLoadIdentity, void(GL_FUNCPTR *)(void), "glLoadIdentity");
        LOAD_FUNCTION_NAME(_ptr_glLoadMatrixf, void(GL_FUNCPTR*)(const GLfloat*), "glLoadMatrixf");
		LOAD_FUNCTION_NAME(_ptr_glBegin, void(GL_FUNCPTR *)(GLenum), "glBegin");
		LOAD_FUNCTION_NAME(_ptr_glEnd, void(GL_FUNCPTR *)(void), "glEnd");
		LOAD_FUNCTION_NAME(_ptr_glColor3f, void(GL_FUNCPTR *)(GLfloat, GLfloat, GLfloat), "glColor3f");
		LOAD_FUNCTION_NAME(_ptr_glVertex2f, void(GL_FUNCPTR *)(GLfloat, GLfloat), "glVertex2f");
		LOAD_FUNCTION_NAME(_ptr_glVertex2i, void(GL_FUNCPTR *)(GLint, GLint), "glVertex2i");
	}
	else{
        _ptr_glMatrixMode = VoidCoreProfileError < GLenum > ;
        _ptr_glLoadIdentity = VoidCoreProfileError < > ;
        _ptr_glLoadMatrixf = VoidCoreProfileError < const GLfloat* >;
		_ptr_glBegin = VoidCoreProfileError < GLenum > ;
		_ptr_glEnd = VoidCoreProfileError < >;
		_ptr_glColor3f = VoidCoreProfileError < GLfloat, GLfloat, GLfloat >;
		_ptr_glVertex2f = VoidCoreProfileError < GLfloat, GLfloat >;
		_ptr_glVertex2i = VoidCoreProfileError < GLint, GLint >;
	}
	LOAD_FUNCTION_NAME(_ptr_glCullFace, void(GL_FUNCPTR*)(GLenum mode), "glCullFace");
	LOAD_FUNCTION_NAME(_ptr_glFrontFace, void(GL_FUNCPTR*)(GLenum mode), "glFrontFace");
	LOAD_FUNCTION_NAME(_ptr_glHint, void(GL_FUNCPTR*)(GLenum target, GLenum mode), "glHint");
	LOAD_FUNCTION_NAME(_ptr_glLineWidth, void(GL_FUNCPTR*)(GLfloat width), "glLineWidth");
	LOAD_FUNCTION_NAME(_ptr_glPointSize, void(GL_FUNCPTR*)(GLfloat size), "glPointSize");
	LOAD_FUNCTION_NAME(_ptr_glPolygonMode, void(GL_FUNCPTR*)(GLenum face, GLenum mode), "glPolygonMode");
	LOAD_FUNCTION_NAME(_ptr_glScissor, void(GL_FUNCPTR*)(GLint x, GLint y, GLsizei width, GLsizei height), "glScissor");
	LOAD_FUNCTION_NAME(_ptr_glTexParameterf, void(GL_FUNCPTR*)(GLenum target, GLenum pname, GLfloat param), "glTexParameterf");
	LOAD_FUNCTION_NAME(_ptr_glTexParameterfv, void(GL_FUNCPTR*)(GLenum target, GLenum pname, const GLfloat *params), "glTexParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glTexParameteri, void(GL_FUNCPTR*)(GLenum target, GLenum pname, GLint param), "glTexParameteri");
	LOAD_FUNCTION_NAME(_ptr_glTexParameteriv, void(GL_FUNCPTR*)(GLenum target, GLenum pname, const GLint *params), "glTexParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glTexImage1D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels), "glTexImage1D");
	LOAD_FUNCTION_NAME(_ptr_glTexImage2D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels), "glTexImage2D");
	LOAD_FUNCTION_NAME(_ptr_glDrawBuffer, void(GL_FUNCPTR*)(GLenum buf), "glDrawBuffer");
	LOAD_FUNCTION_NAME(_ptr_glClear, void(GL_FUNCPTR*)(GLbitfield mask), "glClear");
	LOAD_FUNCTION_NAME(_ptr_glClearColor, void(GL_FUNCPTR*)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), "glClearColor");
	LOAD_FUNCTION_NAME(_ptr_glClearStencil, void(GL_FUNCPTR*)(GLint s), "glClearStencil");
	LOAD_FUNCTION_NAME(_ptr_glClearDepth, void(GL_FUNCPTR*)(GLclampd depth), "glClearDepth");
	LOAD_FUNCTION_NAME(_ptr_glStencilMask, void(GL_FUNCPTR*)(GLuint mask), "glStencilMask");
	LOAD_FUNCTION_NAME(_ptr_glColorMask, void(GL_FUNCPTR*)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha), "glColorMask");
	LOAD_FUNCTION_NAME(_ptr_glDepthMask, void(GL_FUNCPTR*)(GLboolean flag), "glDepthMask");
	LOAD_FUNCTION_NAME(_ptr_glDisable, void(GL_FUNCPTR*)(GLenum cap), "glDisable");
	LOAD_FUNCTION_NAME(_ptr_glEnable, void(GL_FUNCPTR*)(GLenum cap), "glEnable");
	LOAD_FUNCTION_NAME(_ptr_glFinish, void(GL_FUNCPTR*)(void), "glFinish");
	LOAD_FUNCTION_NAME(_ptr_glFlush, void(GL_FUNCPTR*)(void), "glFlush");
	LOAD_FUNCTION_NAME(_ptr_glBlendFunc, void(GL_FUNCPTR*)(GLenum sfactor, GLenum dfactor), "glBlendFunc");
	LOAD_FUNCTION_NAME(_ptr_glLogicOp, void(GL_FUNCPTR*)(GLenum opcode), "glLogicOp");
	LOAD_FUNCTION_NAME(_ptr_glStencilFunc, void(GL_FUNCPTR*)(GLenum func, GLint ref, GLuint mask), "glStencilFunc");
	LOAD_FUNCTION_NAME(_ptr_glStencilOp, void(GL_FUNCPTR*)(GLenum fail, GLenum zfail, GLenum zpass), "glStencilOp");
	LOAD_FUNCTION_NAME(_ptr_glDepthFunc, void(GL_FUNCPTR*)(GLenum func), "glDepthFunc");
	LOAD_FUNCTION_NAME(_ptr_glPixelStoref, void(GL_FUNCPTR*)(GLenum pname, GLfloat param), "glPixelStoref");
	LOAD_FUNCTION_NAME(_ptr_glPixelStorei, void(GL_FUNCPTR*)(GLenum pname, GLint param), "glPixelStorei");
	LOAD_FUNCTION_NAME(_ptr_glReadBuffer, void(GL_FUNCPTR*)(GLenum src), "glReadBuffer");
	LOAD_FUNCTION_NAME(_ptr_glReadPixels, void(GL_FUNCPTR*)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels), "glReadPixels");
	LOAD_FUNCTION_NAME(_ptr_glGetBooleanv, void(GL_FUNCPTR*)(GLenum pname, GLboolean *data), "glGetBooleanv");
	LOAD_FUNCTION_NAME(_ptr_glGetDoublev, void(GL_FUNCPTR*)(GLenum pname, GLdouble *data), "glGetDoublev");
	LOAD_FUNCTION_NAME(_ptr_glGetError, GLenum(GL_FUNCPTR*)(void), "glGetError");
	LOAD_FUNCTION_NAME(_ptr_glGetFloatv, void(GL_FUNCPTR*)(GLenum pname, GLfloat *data), "glGetFloatv");
	LOAD_FUNCTION_NAME(_ptr_glGetIntegerv, void(GL_FUNCPTR*)(GLenum pname, GLint *data), "glGetIntegerv");
	LOAD_FUNCTION_NAME(_ptr_glGetString, const GLubyte *(GL_FUNCPTR*)(GLenum name), "glGetString");
	LOAD_FUNCTION_NAME(_ptr_glGetTexImage, void(GL_FUNCPTR*)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels), "glGetTexImage");
	LOAD_FUNCTION_NAME(_ptr_glGetTexParameterfv, void(GL_FUNCPTR*)(GLenum target, GLenum pname, GLfloat *params), "glGetTexParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glGetTexParameteriv, void(GL_FUNCPTR*)(GLenum target, GLenum pname, GLint *params), "glGetTexParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glGetTexLevelParameterfv, void(GL_FUNCPTR*)(GLenum target, GLint level, GLenum pname, GLfloat *params), "glGetTexLevelParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glGetTexLevelParameteriv, void(GL_FUNCPTR*)(GLenum target, GLint level, GLenum pname, GLint *params), "glGetTexLevelParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glIsEnabled, GLboolean(GL_FUNCPTR*)(GLenum cap), "glIsEnabled");
	LOAD_FUNCTION_NAME(_ptr_glDepthRange, void(GL_FUNCPTR*)(GLdouble near, GLdouble far), "glDepthRange");
	LOAD_FUNCTION_NAME(_ptr_glViewport, void(GL_FUNCPTR*)(GLint x, GLint y, GLsizei width, GLsizei height), "glViewport");
	return numFailed;
}

static int Load_GL_Version_1_1(bool _coreProfile){
	int numFailed = Load_GL_Version_1_0(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glDrawArrays, void(GL_FUNCPTR*)(GLenum mode, GLint first, GLsizei count), "glDrawArrays");
	LOAD_FUNCTION_NAME(_ptr_glDrawElements, void(GL_FUNCPTR*)(GLenum mode, GLsizei count, GLenum type, const void *indices), "glDrawElements");
	LOAD_FUNCTION_NAME(_ptr_glGetPointerv, void(GL_FUNCPTR*)(GLenum pname, void **params), "glGetPointerv");
	LOAD_FUNCTION_NAME(_ptr_glPolygonOffset, void(GL_FUNCPTR*)(GLfloat factor, GLfloat units), "glPolygonOffset");
	LOAD_FUNCTION_NAME(_ptr_glCopyTexImage1D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border), "glCopyTexImage1D");
	LOAD_FUNCTION_NAME(_ptr_glCopyTexImage2D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border), "glCopyTexImage2D");
	LOAD_FUNCTION_NAME(_ptr_glCopyTexSubImage1D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width), "glCopyTexSubImage1D");
	LOAD_FUNCTION_NAME(_ptr_glCopyTexSubImage2D, void (GL_FUNCPTR*)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height), "glCopyTexSubImage2D");
	LOAD_FUNCTION_NAME(_ptr_glTexSubImage1D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels), "glTexSubImage1D");
	LOAD_FUNCTION_NAME(_ptr_glTexSubImage2D, void(GL_FUNCPTR*)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels), "glTexSubImage2D");
	LOAD_FUNCTION_NAME(_ptr_glBindTexture, void(GL_FUNCPTR*)(GLenum target, GLuint texture), "glBindTexture");
	LOAD_FUNCTION_NAME(_ptr_glDeleteTextures, void(GL_FUNCPTR*)(GLsizei n, const GLuint *textures), "glDeleteTextures");
	LOAD_FUNCTION_NAME(_ptr_glGenTextures, void(GL_FUNCPTR*)(GLsizei n, GLuint *textures), "glGenTextures");
	LOAD_FUNCTION_NAME(_ptr_glIsTexture, GLboolean(GL_FUNCPTR*)(GLuint texture), "glIsTexture");
	return numFailed;
}

static int Load_GL_Version_1_2(bool _coreProfile){
	int numFailed = Load_GL_Version_1_1(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glDrawRangeElements, void(GL_FUNCPTR *)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices), "glDrawRangeElements");
	LOAD_FUNCTION_NAME(_ptr_glTexImage3D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels), "glTexImage3D");
	LOAD_FUNCTION_NAME(_ptr_glTexSubImage3D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels), "glTexSubImage3D");
	LOAD_FUNCTION_NAME(_ptr_glCopyTexSubImage3D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height), "glCopyTexSubImage3D");
	return numFailed;
}

static int Load_GL_Version_1_3(bool _coreProfile){
	int numFailed = Load_GL_Version_1_2(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glActiveTexture, void(GL_FUNCPTR *)(GLenum texture), "glActiveTexture");
	LOAD_FUNCTION_NAME(_ptr_glSampleCoverage, void(GL_FUNCPTR *)(GLfloat value, GLboolean invert), "glSampleCoverage");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexImage3D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data), "glCompressedTexImage3D");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexImage2D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data), "glCompressedTexImage2D");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexImage1D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data), "glCompressedTexImage1D");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexSubImage3D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data), "glCompressedTexSubImage3D");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexSubImage2D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data), "glCompressedTexSubImage2D");
	LOAD_FUNCTION_NAME(_ptr_glCompressedTexSubImage1D, void(GL_FUNCPTR *)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data), "glCompressedTexSubImage1D");
	LOAD_FUNCTION_NAME(_ptr_glGetCompressedTexImage, void(GL_FUNCPTR *)(GLenum target, GLint level, void *img), "glGetCompressedTexImage");
	return numFailed;
}

static int Load_GL_Version_1_4(bool _coreProfile){
	int numFailed = Load_GL_Version_1_3(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glBlendFuncSeparate, void(GL_FUNCPTR *)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha), "glBlendFuncSeparate");
	LOAD_FUNCTION_NAME(_ptr_glMultiDrawArrays, void(GL_FUNCPTR *)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount), "glMultiDrawArrays");
	LOAD_FUNCTION_NAME(_ptr_glMultiDrawElements, void(GL_FUNCPTR *)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount), "glMultiDrawElements");
	LOAD_FUNCTION_NAME(_ptr_glPointParameterf, void(GL_FUNCPTR *)(GLenum pname, GLfloat param), "glPointParameterf");
	LOAD_FUNCTION_NAME(_ptr_glPointParameterfv, void(GL_FUNCPTR *)(GLenum pname, const GLfloat *params), "glPointParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glPointParameteri, void(GL_FUNCPTR *)(GLenum pname, GLint param), "glPointParameteri");
	LOAD_FUNCTION_NAME(_ptr_glPointParameteriv, void(GL_FUNCPTR *)(GLenum pname, const GLint *params), "glPointParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glBlendColor, void(GL_FUNCPTR *)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), "glBlendColor");
	LOAD_FUNCTION_NAME(_ptr_glBlendEquation, void(GL_FUNCPTR *)(GLenum mode), "glBlendEquation");
	return numFailed;
}

static int Load_GL_Version_1_5(bool _coreProfile){
	int numFailed = Load_GL_Version_1_4(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glGenQueries, void(GL_FUNCPTR *)(GLsizei n, GLuint *ids), "glGenQueries");
	LOAD_FUNCTION_NAME(_ptr_glDeleteQueries, void(GL_FUNCPTR *)(GLsizei n, const GLuint *ids), "glDeleteQueries");
	LOAD_FUNCTION_NAME(_ptr_glIsQuery, GLboolean(GL_FUNCPTR *)(GLuint id), "glIsQuery");
	LOAD_FUNCTION_NAME(_ptr_glBeginQuery, void(GL_FUNCPTR *)(GLenum target, GLuint id), "glBeginQuery");
	LOAD_FUNCTION_NAME(_ptr_glEndQuery, void(GL_FUNCPTR *)(GLenum target), "glEndQuery");
	LOAD_FUNCTION_NAME(_ptr_glGetQueryiv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLint *params), "glGetQueryiv");
	LOAD_FUNCTION_NAME(_ptr_glGetQueryObjectiv, void(GL_FUNCPTR *)(GLuint id, GLenum pname, GLint *params), "glGetQueryObjectiv");
	LOAD_FUNCTION_NAME(_ptr_glGetQueryObjectuiv, void(GL_FUNCPTR *)(GLuint id, GLenum pname, GLuint *params), "glGetQueryObjectuiv");
	LOAD_FUNCTION_NAME(_ptr_glBindBuffer, void(GL_FUNCPTR *)(GLenum target, GLuint buffer), "glBindBuffer");
	LOAD_FUNCTION_NAME(_ptr_glDeleteBuffers, void(GL_FUNCPTR *)(GLsizei n, const GLuint *buffers), "glDeleteBuffers");
	LOAD_FUNCTION_NAME(_ptr_glGenBuffers, void(GL_FUNCPTR *)(GLsizei n, GLuint *buffers), "glGenBuffers");
	LOAD_FUNCTION_NAME(_ptr_glIsBuffer, GLboolean(GL_FUNCPTR *)(GLuint buffer), "glIsBuffer");
	LOAD_FUNCTION_NAME(_ptr_glBufferData, void(GL_FUNCPTR *)(GLenum target, GLsizeiptr size, const void *data, GLenum usage), "glBufferData");
	LOAD_FUNCTION_NAME(_ptr_glBufferSubData, void(GL_FUNCPTR *)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data), "glBufferSubData");
	LOAD_FUNCTION_NAME(_ptr_glGetBufferSubData, void(GL_FUNCPTR *)(GLenum target, GLintptr offset, GLsizeiptr size, void *data), "glGetBufferSubData");
	LOAD_FUNCTION_NAME(_ptr_glMapBuffer, void *(GL_FUNCPTR *)(GLenum target, GLenum access), "glMapBuffer");
	LOAD_FUNCTION_NAME(_ptr_glUnmapBuffer, GLboolean(GL_FUNCPTR *)(GLenum target), "glUnmapBuffer");
	LOAD_FUNCTION_NAME(_ptr_glGetBufferParameteriv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLint *params), "glGetBufferParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glGetBufferPointerv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, void **params), "glGetBufferPointerv");
	return numFailed;
}

static int Load_GL_Version_2_0(bool _coreProfile){
	int numFailed = Load_GL_Version_1_5(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glBlendEquationSeparate, void(GL_FUNCPTR *)(GLenum modeRGB, GLenum modeAlpha), "glBlendEquationSeparate");
	LOAD_FUNCTION_NAME(_ptr_glDrawBuffers, void(GL_FUNCPTR *)(GLsizei n, const GLenum *bufs), "glDrawBuffers");
	LOAD_FUNCTION_NAME(_ptr_glStencilOpSeparate, void(GL_FUNCPTR *)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass), "glStencilOpSeparate");
	LOAD_FUNCTION_NAME(_ptr_glStencilFuncSeparate, void (GL_FUNCPTR *)(GLenum face, GLenum func, GLint ref, GLuint mask), "glStencilFuncSeparate");
	LOAD_FUNCTION_NAME(_ptr_glStencilMaskSeparate, void(GL_FUNCPTR *)(GLenum face, GLuint mask), "glStencilMaskSeparate");
	LOAD_FUNCTION_NAME(_ptr_glAttachShader, void(GL_FUNCPTR *)(GLuint program, GLuint shader), "glAttachShader");
	LOAD_FUNCTION_NAME(_ptr_glBindAttribLocation, void(GL_FUNCPTR *)(GLuint program, GLuint index, const GLchar *name), "glBindAttribLocation");
	LOAD_FUNCTION_NAME(_ptr_glCompileShader, void(GL_FUNCPTR *)(GLuint shader), "glCompileShader");
	LOAD_FUNCTION_NAME(_ptr_glCreateProgram, GLuint(GL_FUNCPTR *)(void), "glCreateProgram");
	LOAD_FUNCTION_NAME(_ptr_glCreateShader, GLuint(GL_FUNCPTR *)(GLenum type), "glCreateShader");
	LOAD_FUNCTION_NAME(_ptr_glDeleteProgram, void(GL_FUNCPTR *)(GLuint program), "glDeleteProgram");
	LOAD_FUNCTION_NAME(_ptr_glDeleteShader, void(GL_FUNCPTR *)(GLuint shader), "glDeleteShader");
	LOAD_FUNCTION_NAME(_ptr_glDetachShader, void(GL_FUNCPTR *)(GLuint program, GLuint shader), "glDetachShader");
	LOAD_FUNCTION_NAME(_ptr_glDisableVertexAttribArray, void(GL_FUNCPTR *)(GLuint index), "glDisableVertexAttribArray");
	LOAD_FUNCTION_NAME(_ptr_glEnableVertexAttribArray, void(GL_FUNCPTR *)(GLuint index), "glEnableVertexAttribArray");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveAttrib, void(GL_FUNCPTR *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name), "glGetActiveAttrib");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveUniform, void(GL_FUNCPTR *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name), "glGetActiveUniform");
	LOAD_FUNCTION_NAME(_ptr_glGetAttachedShaders, void(GL_FUNCPTR *)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders), "glGetAttachedShaders");
	LOAD_FUNCTION_NAME(_ptr_glGetAttribLocation, GLint(GL_FUNCPTR *)(GLuint program, const GLchar *name), "glGetAttribLocation");
	LOAD_FUNCTION_NAME(_ptr_glGetProgramiv, void(GL_FUNCPTR *)(GLuint program, GLenum pname, GLint *params), "glGetProgramiv");
	LOAD_FUNCTION_NAME(_ptr_glGetProgramInfoLog, void(GL_FUNCPTR *)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog), "glGetProgramInfoLog");
	LOAD_FUNCTION_NAME(_ptr_glGetShaderiv, void(GL_FUNCPTR *)(GLuint shader, GLenum pname, GLint *params), "glGetShaderiv");
	LOAD_FUNCTION_NAME(_ptr_glGetShaderInfoLog, void(GL_FUNCPTR *)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog), "glGetShaderInfoLog");
	LOAD_FUNCTION_NAME(_ptr_glGetShaderSource, void(GL_FUNCPTR *)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source), "glGetShaderSource");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformLocation, GLint(GL_FUNCPTR *)(GLuint program, const GLchar *name), "glGetUniformLocation");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformfv, void(GL_FUNCPTR *)(GLuint program, GLint location, GLfloat *params), "glGetUniformfv");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformiv, void(GL_FUNCPTR *)(GLuint program, GLint location, GLint *params), "glGetUniformiv");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribdv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, GLdouble *params), "glGetVertexAttribdv");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribfv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, GLfloat *params), "glGetVertexAttribfv");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribiv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, GLint *params), "glGetVertexAttribiv");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribPointerv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, void **pointer), "glGetVertexAttribPointerv");
	LOAD_FUNCTION_NAME(_ptr_glIsProgram, GLboolean(GL_FUNCPTR *)(GLuint program), "glIsProgram");
	LOAD_FUNCTION_NAME(_ptr_glIsShader, GLboolean(GL_FUNCPTR *)(GLuint shader), "glIsShader");
	LOAD_FUNCTION_NAME(_ptr_glLinkProgram, void(GL_FUNCPTR *)(GLuint program), "glLinkProgram");
	LOAD_FUNCTION_NAME(_ptr_glShaderSource, void(GL_FUNCPTR *)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length), "glShaderSource");
	LOAD_FUNCTION_NAME(_ptr_glUseProgram, void(GL_FUNCPTR *)(GLuint program), "glUseProgram");
	LOAD_FUNCTION_NAME(_ptr_glUniform1f, void(GL_FUNCPTR *)(GLint location, GLfloat v0), "glUniform1f");
	LOAD_FUNCTION_NAME(_ptr_glUniform2f, void(GL_FUNCPTR *)(GLint location, GLfloat v0, GLfloat v1), "glUniform2f");
	LOAD_FUNCTION_NAME(_ptr_glUniform3f, void(GL_FUNCPTR *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2), "glUniform3f");
	LOAD_FUNCTION_NAME(_ptr_glUniform4f, void(GL_FUNCPTR *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3), "glUniform4f");
	LOAD_FUNCTION_NAME(_ptr_glUniform1i, void(GL_FUNCPTR *)(GLint location, GLint v0), "glUniform1i");
	LOAD_FUNCTION_NAME(_ptr_glUniform2i, void(GL_FUNCPTR *)(GLint location, GLint v0, GLint v1), "glUniform2i");
	LOAD_FUNCTION_NAME(_ptr_glUniform3i, void(GL_FUNCPTR *)(GLint location, GLint v0, GLint v1, GLint v2), "glUniform3i");
	LOAD_FUNCTION_NAME(_ptr_glUniform4i, void(GL_FUNCPTR *)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3), "glUniform4i");
	LOAD_FUNCTION_NAME(_ptr_glUniform1fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLfloat *value), "glUniform1fv");
	LOAD_FUNCTION_NAME(_ptr_glUniform2fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLfloat *value), "glUniform2fv");
	LOAD_FUNCTION_NAME(_ptr_glUniform3fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLfloat *value), "glUniform3fv");
	LOAD_FUNCTION_NAME(_ptr_glUniform4fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLfloat *value), "glUniform4fv");
	LOAD_FUNCTION_NAME(_ptr_glUniform1iv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLint *value), "glUniform1iv");
	LOAD_FUNCTION_NAME(_ptr_glUniform2iv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLint *value), "glUniform2iv");
	LOAD_FUNCTION_NAME(_ptr_glUniform3iv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLint *value), "glUniform3iv");
	LOAD_FUNCTION_NAME(_ptr_glUniform4iv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLint *value), "glUniform3iv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix2fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix2fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix3fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix3fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix4fv, void(GL_FUNCPTR *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix4fv");
	LOAD_FUNCTION_NAME(_ptr_glValidateProgram, void(GL_FUNCPTR *)(GLuint program), "glValidateProgram");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1d, void(GL_FUNCPTR *)(GLuint index, GLdouble x), "glVertexAttrib1d");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1dv, void(GL_FUNCPTR *)(GLuint index, const GLdouble *v), "glVertexAttrib1dv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1f, void(GL_FUNCPTR *)(GLuint index, GLfloat x), "glVertexAttrib1f");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1fv, void(GL_FUNCPTR *)(GLuint index, const GLfloat *v), "glVertexAttrib1fv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1s, void(GL_FUNCPTR *)(GLuint index, GLshort x), "glVertexAttrib1s");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib1sv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttrib1sv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2d, void(GL_FUNCPTR *)(GLuint index, GLdouble x, GLdouble y), "glVertexAttrib2d");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2dv, void(GL_FUNCPTR *)(GLuint index, const GLdouble *v), "glVertexAttrib2dv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2f, void(GL_FUNCPTR *)(GLuint index, GLfloat x, GLfloat y), "glVertexAttrib2f");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2fv, void(GL_FUNCPTR *)(GLuint index, const GLfloat *v), "glVertexAttrib2fv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2s, void(GL_FUNCPTR *)(GLuint index, GLshort x, GLshort y), "glVertexAttrib2s");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib2sv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttrib2sv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3d, void(GL_FUNCPTR *)(GLuint index, GLdouble x, GLdouble y, GLdouble z), "glVertexAttrib3d");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3dv, void(GL_FUNCPTR *)(GLuint index, const GLdouble *v), "glVertexAttrib3dv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3f, void(GL_FUNCPTR *)(GLuint index, GLfloat x, GLfloat y, GLfloat z), "glVertexAttrib3f");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3fv, void(GL_FUNCPTR *)(GLuint index, const GLfloat *v), "glVertexAttrib3fv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3s, void(GL_FUNCPTR *)(GLuint index, GLshort x, GLshort y, GLshort z), "glVertexAttrib3s");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib3sv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttrib3sv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nbv, void(GL_FUNCPTR *)(GLuint index, const GLbyte *v), "glVertexAttrib4Nbv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Niv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttrib4Niv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nsv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttrib4Nsv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nub, void(GL_FUNCPTR *)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w), "glVertexAttrib4Nub");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nubv, void(GL_FUNCPTR *)(GLuint index, const GLubyte *v), "glVertexAttrib4Nubv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nuiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttrib4Nuiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4Nusv, void(GL_FUNCPTR *)(GLuint index, const GLushort *v), "glVertexAttrib4Nusv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4bv, void(GL_FUNCPTR *)(GLuint index, const GLbyte *v), "glVertexAttrib4bv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4d, void(GL_FUNCPTR *)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w), "glVertexAttrib4d");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4dv, void(GL_FUNCPTR *)(GLuint index, const GLdouble *v), "glVertexAttrib4dv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4f, void(GL_FUNCPTR *)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w), "glVertexAttrib4f");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4fv, void(GL_FUNCPTR *)(GLuint index, const GLfloat *v), "glVertexAttrib4fv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4iv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttrib4iv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4s, void(GL_FUNCPTR *)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w), "glVertexAttrib4s");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4sv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttrib4sv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4ubv, void(GL_FUNCPTR *)(GLuint index, const GLubyte *v), "glVertexAttrib4ubv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4uiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttrib4uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttrib4usv, void(GL_FUNCPTR *)(GLuint index, const GLushort *v), "glVertexAttrib4usv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribPointer, void(GL_FUNCPTR *)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer), "glVertexAttribPointer");
	return numFailed;
}

static int Load_GL_Version_2_1(bool _coreProfile){
	int numFailed = Load_GL_Version_2_0(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix2x3fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix2x3fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix3x2fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix3x2fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix2x4fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix2x4fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix4x2fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix4x2fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix3x4fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix3x4fv");
	LOAD_FUNCTION_NAME(_ptr_glUniformMatrix4x3fv, void(GL_FUNCPTR*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value), "glUniformMatrix4x3fv");
	return numFailed;
}

static int Load_GL_Version_3_0(bool _coreProfile){
	int numFailed = Load_GL_Version_2_1(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glColorMaski, void(GL_FUNCPTR *)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a), "glColorMaski");
	LOAD_FUNCTION_NAME(_ptr_glGetBooleani_v, void(GL_FUNCPTR *)(GLenum target, GLuint index, GLboolean *data), "glGetBooleani_v");
	LOAD_FUNCTION_NAME(_ptr_glGetIntegeri_v, void(GL_FUNCPTR *)(GLenum target, GLuint index, GLint *data), "glGetIntegeri_v");
	LOAD_FUNCTION_NAME(_ptr_glEnablei, void(GL_FUNCPTR *)(GLenum target, GLuint index), "glEnablei");
	LOAD_FUNCTION_NAME(_ptr_glDisablei, void(GL_FUNCPTR *)(GLenum target, GLuint index), "glDisablei");
	LOAD_FUNCTION_NAME(_ptr_glIsEnabledi, GLboolean(GL_FUNCPTR *)(GLenum target, GLuint index), "glIsEnabledi");
	LOAD_FUNCTION_NAME(_ptr_glBeginTransformFeedback, void(GL_FUNCPTR *)(GLenum primitiveMode), "glBeginTransformFeedback");
	LOAD_FUNCTION_NAME(_ptr_glEndTransformFeedback, void(GL_FUNCPTR *)(void), "glEndTransformFeedback");
	LOAD_FUNCTION_NAME(_ptr_glBindBufferRange, void(GL_FUNCPTR *)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size), "glBindBufferRange");
	LOAD_FUNCTION_NAME(_ptr_glBindBufferBase, void(GL_FUNCPTR *)(GLenum target, GLuint index, GLuint buffer), "glBindBufferBase");
	LOAD_FUNCTION_NAME(_ptr_glTransformFeedbackVaryings, void(GL_FUNCPTR *)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode), "glTransformFeedbackVaryings");
	LOAD_FUNCTION_NAME(_ptr_glGetTransformFeedbackVarying, void(GL_FUNCPTR *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name), "glGetTransformFeedbackVarying");
	LOAD_FUNCTION_NAME(_ptr_glClampColor, void(GL_FUNCPTR *)(GLenum target, GLenum clamp), "glClampColor");
	LOAD_FUNCTION_NAME(_ptr_glBeginConditionalRender, void(GL_FUNCPTR *)(GLuint id, GLenum mode), "glBeginConditionalRender");
	LOAD_FUNCTION_NAME(_ptr_glEndConditionalRender, void(GL_FUNCPTR *)(void), "glEndConditionalRender");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribIPointer, void(GL_FUNCPTR *)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer), "glVertexAttribIPointer");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribIiv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, GLint *params), "glGetVertexAttribIiv");
	LOAD_FUNCTION_NAME(_ptr_glGetVertexAttribIuiv, void(GL_FUNCPTR *)(GLuint index, GLenum pname, GLuint *params), "glGetVertexAttribIuiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI1i, void(GL_FUNCPTR *)(GLuint index, GLint x), "glVertexAttribI1i");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI2i, void(GL_FUNCPTR *)(GLuint index, GLint x, GLint y), "glVertexAttribI2i");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI3i, void(GL_FUNCPTR *)(GLuint index, GLint x, GLint y, GLint z), "glVertexAttribI3i");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4i, void(GL_FUNCPTR *)(GLuint index, GLint x, GLint y, GLint z, GLint w), "glVertexAttribI4i");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI1ui, void(GL_FUNCPTR *)(GLuint index, GLuint x), "glVertexAttribI1ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI2ui, void(GL_FUNCPTR *)(GLuint index, GLuint x, GLuint y), "glVertexAttribI2ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI3ui, void(GL_FUNCPTR *)(GLuint index, GLuint x, GLuint y, GLuint z), "glVertexAttribI3ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4ui, void(GL_FUNCPTR *)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w), "glVertexAttribI4ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI1iv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttribI1iv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI2iv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttribI2iv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI3iv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttribI3iv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4iv, void(GL_FUNCPTR *)(GLuint index, const GLint *v), "glVertexAttribI4iv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI1uiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttribI1uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI2uiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttribI2uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI3uiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttribI3uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4uiv, void(GL_FUNCPTR *)(GLuint index, const GLuint *v), "glVertexAttribI4uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4bv, void(GL_FUNCPTR *)(GLuint index, const GLbyte *v), "glVertexAttribI4bv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4sv, void(GL_FUNCPTR *)(GLuint index, const GLshort *v), "glVertexAttribI4sv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4ubv, void(GL_FUNCPTR *)(GLuint index, const GLubyte *v), "glVertexAttribI4ubv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribI4usv, void(GL_FUNCPTR *)(GLuint index, const GLushort *v), "glVertexAttribI4usv");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformuiv, void(GL_FUNCPTR *)(GLuint program, GLint location, GLuint *params), "glGetUniformuiv");
	LOAD_FUNCTION_NAME(_ptr_glBindFragDataLocation, void(GL_FUNCPTR *)(GLuint program, GLuint color, const GLchar *name), "glBindFragDataLocation");
	LOAD_FUNCTION_NAME(_ptr_glGetFragDataLocation, GLint(GL_FUNCPTR *)(GLuint program, const GLchar *name), "glGetFragDataLocation");
	LOAD_FUNCTION_NAME(_ptr_glUniform1ui, void(GL_FUNCPTR *)(GLint location, GLuint v0), "glUniform1ui");
	LOAD_FUNCTION_NAME(_ptr_glUniform2ui, void(GL_FUNCPTR *)(GLint location, GLuint v0, GLuint v1), "glUniform2ui");
	LOAD_FUNCTION_NAME(_ptr_glUniform3ui, void(GL_FUNCPTR *)(GLint location, GLuint v0, GLuint v1, GLuint v2), "glUniform3ui");
	LOAD_FUNCTION_NAME(_ptr_glUniform4ui, void(GL_FUNCPTR *)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3), "glUniform4ui");
	LOAD_FUNCTION_NAME(_ptr_glUniform1uiv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLuint *value), "glUniform1uiv");
	LOAD_FUNCTION_NAME(_ptr_glUniform2uiv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLuint *value), "glUniform2uiv");
	LOAD_FUNCTION_NAME(_ptr_glUniform3uiv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLuint *value), "glUniform3uiv");
	LOAD_FUNCTION_NAME(_ptr_glUniform4uiv, void(GL_FUNCPTR *)(GLint location, GLsizei count, const GLuint *value), "glUniform4uiv");
	LOAD_FUNCTION_NAME(_ptr_glTexParameterIiv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, const GLint *params), "glTexParameterIiv");
	LOAD_FUNCTION_NAME(_ptr_glTexParameterIuiv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, const GLuint *params), "glTexParameterIuiv");
	LOAD_FUNCTION_NAME(_ptr_glGetTexParameterIiv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLint *params), "glGetTexParameterIiv");
	LOAD_FUNCTION_NAME(_ptr_glGetTexParameterIuiv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLuint *params), "glGetTexParameterIuiv");
	LOAD_FUNCTION_NAME(_ptr_glClearBufferiv, void(GL_FUNCPTR *)(GLenum buffer, GLint drawbuffer, const GLint *value), "glClearBufferiv");
	LOAD_FUNCTION_NAME(_ptr_glClearBufferuiv, void(GL_FUNCPTR *)(GLenum buffer, GLint drawbuffer, const GLuint *value), "glClearBufferuiv");
	LOAD_FUNCTION_NAME(_ptr_glClearBufferfv, void(GL_FUNCPTR *)(GLenum buffer, GLint drawbuffer, const GLfloat *value), "glClearBufferfv");
	LOAD_FUNCTION_NAME(_ptr_glClearBufferfi, void(GL_FUNCPTR *)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil), "glClearBufferfi");
	LOAD_FUNCTION_NAME(_ptr_glGetStringi, const GLubyte *(GL_FUNCPTR *)(GLenum name, GLuint index), "glGetString");
	LOAD_FUNCTION_NAME(_ptr_glIsRenderbuffer, GLboolean(GL_FUNCPTR *)(GLuint renderbuffer), "glIsRenderbuffer");
	LOAD_FUNCTION_NAME(_ptr_glBindRenderbuffer, void(GL_FUNCPTR *)(GLenum target, GLuint renderbuffer), "glBindRenderbuffer");
	LOAD_FUNCTION_NAME(_ptr_glDeleteRenderbuffers, void(GL_FUNCPTR *)(GLsizei n, const GLuint *renderbuffers), "glDeleteRenderbuffers");
	LOAD_FUNCTION_NAME(_ptr_glGenRenderbuffers, void(GL_FUNCPTR *)(GLsizei n, GLuint *renderbuffers), "glGenRenderbuffers");
	LOAD_FUNCTION_NAME(_ptr_glRenderbufferStorage, void(GL_FUNCPTR *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height), "glRenderbufferStorage");
	LOAD_FUNCTION_NAME(_ptr_glGetRenderbufferParameteriv, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLint *params), "glGetRenderbufferParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glIsFramebuffer, GLboolean(GL_FUNCPTR *)(GLuint framebuffer), "glIsFramebuffer");
	LOAD_FUNCTION_NAME(_ptr_glBindFramebuffer, void(GL_FUNCPTR *)(GLenum target, GLuint framebuffer), "glBindFramebuffer");
	LOAD_FUNCTION_NAME(_ptr_glDeleteFramebuffers, void(GL_FUNCPTR *)(GLsizei n, const GLuint *framebuffers), "glDeleteFramebuffers");
	LOAD_FUNCTION_NAME(_ptr_glGenFramebuffers, void(GL_FUNCPTR *)(GLsizei n, GLuint *framebuffers), "glGenFramebuffers");
	LOAD_FUNCTION_NAME(_ptr_glCheckFramebufferStatus, GLenum(GL_FUNCPTR *)(GLenum target), "glCheckFramebufferStatus");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferTexture1D, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), "glFramebufferTexture1D");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferTexture2D, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), "glFramebufferTexture2D");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferTexture3D, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset), "glFramebufferTexture3D");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferRenderbuffer, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer), "glFramebufferRenderbuffer");
	LOAD_FUNCTION_NAME(_ptr_glGetFramebufferAttachmentParameteriv, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLenum pname, GLint *params), "glGetFramebufferAttachmentParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glGenerateMipmap, void(GL_FUNCPTR *)(GLenum target), "glGenerateMipmap");
	LOAD_FUNCTION_NAME(_ptr_glBlitFramebuffer, void(GL_FUNCPTR *)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter), "glBlitFramebuffer");
	LOAD_FUNCTION_NAME(_ptr_glRenderbufferStorageMultisample, void(GL_FUNCPTR *)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height), "glRenderbufferStorageMultisample");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferTextureLayer, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer), "glFramebufferTextureLayer");
	LOAD_FUNCTION_NAME(_ptr_glMapBufferRange, void *(GL_FUNCPTR *)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access), "glMapBufferRange");
	LOAD_FUNCTION_NAME(_ptr_glFlushMappedBufferRange, void(GL_FUNCPTR *)(GLenum target, GLintptr offset, GLsizeiptr length), "glFlushMappedBufferRange");
	LOAD_FUNCTION_NAME(_ptr_glBindVertexArray, void(GL_FUNCPTR *)(GLuint _array), "glBindVertexArray");
	LOAD_FUNCTION_NAME(_ptr_glDeleteVertexArrays, void(GL_FUNCPTR *)(GLsizei n, const GLuint *arrays), "glDeleteVertexArrays");
	LOAD_FUNCTION_NAME(_ptr_glGenVertexArrays, void(GL_FUNCPTR *)(GLsizei n, GLuint *arrays), "glGenVertexArrays");
	LOAD_FUNCTION_NAME(_ptr_glIsVertexArray, GLboolean(GL_FUNCPTR *)(GLuint _array), "glIsVertexArray");
	return numFailed;
}

static int Load_GL_Version_3_1(bool _coreProfile){
	int numFailed = Load_GL_Version_3_0(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glDrawArraysInstanced, void(GL_FUNCPTR *)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount), "glDrawArraysInstanced");
	LOAD_FUNCTION_NAME(_ptr_glDrawElementsInstanced, void(GL_FUNCPTR *)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount), "glDrawElementsInstanced");
	LOAD_FUNCTION_NAME(_ptr_glTexBuffer, void(GL_FUNCPTR *)(GLenum target, GLenum internalformat, GLuint buffer), "glTexBuffer");
	LOAD_FUNCTION_NAME(_ptr_glPrimitiveRestartIndex, void(GL_FUNCPTR *)(GLuint index), "glPrimitiveRestartIndex");
	LOAD_FUNCTION_NAME(_ptr_glCopyBufferSubData, void(GL_FUNCPTR *)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size), "glCopyBufferSubData");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformIndices, void(GL_FUNCPTR *)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices), "glGetUniformIndices");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveUniformsiv, void(GL_FUNCPTR *)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params), "glGetActiveUniformsiv");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveUniformName, void(GL_FUNCPTR *)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName), "glGetActiveUniformName");
	LOAD_FUNCTION_NAME(_ptr_glGetUniformBlockIndex, GLuint(GL_FUNCPTR *)(GLuint program, const GLchar *uniformBlockName), "glGetUniformBlockIndex");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveUniformBlockiv, void(GL_FUNCPTR *)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params), "glGetActiveUniformBlockiv");
	LOAD_FUNCTION_NAME(_ptr_glGetActiveUniformBlockName, void(GL_FUNCPTR *)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName), "glGetActiveUniformBlockName");
	LOAD_FUNCTION_NAME(_ptr_glUniformBlockBinding, void(GL_FUNCPTR *)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding), "glUniformBlockBinding");
	return numFailed;
}

static int Load_GL_Version_3_2(bool _coreProfile){
	int numFailed = Load_GL_Version_3_1(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glDrawElementsBaseVertex, void(GL_FUNCPTR *)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex), "glDrawElementsBaseVertex");
	LOAD_FUNCTION_NAME(_ptr_glDrawRangeElementsBaseVertex, void(GL_FUNCPTR *)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex), "glDrawRangeElementsBaseVertex");
	LOAD_FUNCTION_NAME(_ptr_glDrawElementsInstancedBaseVertex, void(GL_FUNCPTR *)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex), "glDrawElementsInstancedBaseVertex");
	LOAD_FUNCTION_NAME(_ptr_glMultiDrawElementsBaseVertex, void(GL_FUNCPTR *)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex), "glMultiDrawElementsBaseVertex");
	LOAD_FUNCTION_NAME(_ptr_glProvokingVertex, void(GL_FUNCPTR *)(GLenum mode), "glProvokingVertex");
	LOAD_FUNCTION_NAME(_ptr_glFenceSync, GLsync(GL_FUNCPTR *)(GLenum condition, GLbitfield flags), "glFenceSync");
	LOAD_FUNCTION_NAME(_ptr_glIsSync, GLboolean(GL_FUNCPTR *)(GLsync sync), "glIsSync");
	LOAD_FUNCTION_NAME(_ptr_glDeleteSync, void(GL_FUNCPTR *)(GLsync sync), "glDeleteSync");
	LOAD_FUNCTION_NAME(_ptr_glClientWaitSync, GLenum(GL_FUNCPTR *)(GLsync sync, GLbitfield flags, GLuint64 timeout), "glClientWaitSync");
	LOAD_FUNCTION_NAME(_ptr_glWaitSync, void(GL_FUNCPTR *)(GLsync sync, GLbitfield flags, GLuint64 timeout), "glWaitSync");
	LOAD_FUNCTION_NAME(_ptr_glGetInteger64v, void(GL_FUNCPTR *)(GLenum pname, GLint64 *data), "glGetInteger64v");
	LOAD_FUNCTION_NAME(_ptr_glGetSynciv, void(GL_FUNCPTR *)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values), "glGetSynciv");
	LOAD_FUNCTION_NAME(_ptr_glGetInteger64i_v, void(GL_FUNCPTR *)(GLenum target, GLuint index, GLint64 *data), "glGetInteger64i_v");
	LOAD_FUNCTION_NAME(_ptr_glGetBufferParameteri64v, void(GL_FUNCPTR *)(GLenum target, GLenum pname, GLint64 *params), "glGetBufferParameteri64v");
	LOAD_FUNCTION_NAME(_ptr_glFramebufferTexture, void(GL_FUNCPTR *)(GLenum target, GLenum attachment, GLuint texture, GLint level), "glFramebufferTexture");
	LOAD_FUNCTION_NAME(_ptr_glTexImage2DMultisample, void(GL_FUNCPTR *)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations), "glTexImage2DMultisample");
	LOAD_FUNCTION_NAME(_ptr_glTexImage3DMultisample, void(GL_FUNCPTR *)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations), "glTexImage3DMultisample");
	LOAD_FUNCTION_NAME(_ptr_glGetMultisamplefv, void(GL_FUNCPTR *)(GLenum pname, GLuint index, GLfloat *val), "glGetMultisamplefv");
	LOAD_FUNCTION_NAME(_ptr_glSampleMaski, void(GL_FUNCPTR *)(GLuint maskNumber, GLbitfield mask), "glSampleMaski");
	return numFailed;
}

static int Load_GL_Version_3_3(bool _coreProfile){
	int numFailed = Load_GL_Version_3_2(_coreProfile);
	if (!_coreProfile){

	}
	LOAD_FUNCTION_NAME(_ptr_glBindFragDataLocationIndexed, void(GL_FUNCPTR *)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name), "glBindFragDataLocationIndexed");
	LOAD_FUNCTION_NAME(_ptr_glGetFragDataIndex, GLint(GL_FUNCPTR *)(GLuint program, const GLchar *name), "glGetFragDataIndex");
	LOAD_FUNCTION_NAME(_ptr_glGenSamplers, void(GL_FUNCPTR *)(GLsizei count, GLuint *samplers), "glGenSamplers");
	LOAD_FUNCTION_NAME(_ptr_glDeleteSamplers, void(GL_FUNCPTR *)(GLsizei count, const GLuint *samplers), "glDeleteSamplers");
	LOAD_FUNCTION_NAME(_ptr_glIsSampler, GLboolean(GL_FUNCPTR *)(GLuint sampler), "glIsSampler");
	LOAD_FUNCTION_NAME(_ptr_glBindSampler, void(GL_FUNCPTR *)(GLuint unit, GLuint sampler), "glBindSampler");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameteri, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLint param), "glSamplerParameteri");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameteriv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, const GLint *param), "glSamplerParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameterf, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLfloat param), "glSamplerParameterf");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameterfv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, const GLfloat *param), "glSamplerParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameterIiv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, const GLint *param), "glSamplerParameterIiv");
	LOAD_FUNCTION_NAME(_ptr_glSamplerParameterIuiv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, const GLuint *param), "glSamplerParameterIuiv");
	LOAD_FUNCTION_NAME(_ptr_glGetSamplerParameteriv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLint *params), "glGetSamplerParameteriv");
	LOAD_FUNCTION_NAME(_ptr_glGetSamplerParameterIiv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLint *params), "glGetSamplerParameterIiv");
	LOAD_FUNCTION_NAME(_ptr_glGetSamplerParameterfv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLfloat *params), "glGetSamplerParameterfv");
	LOAD_FUNCTION_NAME(_ptr_glGetSamplerParameterIuiv, void(GL_FUNCPTR *)(GLuint sampler, GLenum pname, GLuint *params), "glGetSamplerParameterIuiv");
	LOAD_FUNCTION_NAME(_ptr_glQueryCounter, void(GL_FUNCPTR *)(GLuint id, GLenum target), "glQueryCounter");
	LOAD_FUNCTION_NAME(_ptr_glGetQueryObjecti64v, void(GL_FUNCPTR *)(GLuint id, GLenum pname, GLint64 *params), "glGetQueryObjecti64v");
	LOAD_FUNCTION_NAME(_ptr_glGetQueryObjectui64v, void(GL_FUNCPTR *)(GLuint id, GLenum pname, GLuint64 *params), "glGetQueryObjectui64v");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribDivisor, void(GL_FUNCPTR *)(GLuint index, GLuint divisor), "glVertexAttribDivisor");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP1ui, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, GLuint value), "glVertexAttribP1ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP1uiv, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value), "glVertexAttribP1uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP2ui, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, GLuint value), "glVertexAttribP2ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP2uiv, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value), "glVertexAttribP2uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP3ui, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, GLuint value), "glVertexAttribP3ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP3uiv, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value), "glVertexAttribP3uiv");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP4ui, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, GLuint value), "glVertexAttribP4ui");
	LOAD_FUNCTION_NAME(_ptr_glVertexAttribP4uiv, void(GL_FUNCPTR *)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value), "glVertexAttribP4uiv");
	return numFailed;
}

static int Load_GL_Version_4_0(bool _coreProfile){
	int numFailed = Load_GL_Version_3_3(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int Load_GL_Version_4_1(bool _coreProfile){
	int numFailed = Load_GL_Version_4_0(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int Load_GL_Version_4_2(bool _coreProfile){
	int numFailed = Load_GL_Version_4_1(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int Load_GL_Version_4_3(bool _coreProfile){
	int numFailed = Load_GL_Version_4_2(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int Load_GL_Version_4_4(bool _coreProfile){
	int numFailed = Load_GL_Version_4_3(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int Load_GL_Version_4_5(bool _coreProfile){
	int numFailed = Load_GL_Version_4_4(_coreProfile);
	if (!_coreProfile){

	}
	return numFailed;
}

static int LoadOpenGLFunctions(bool _coreProfile){
	return Load_GL_Version_4_5(_coreProfile);
}


/*

#ifndef GL_VERSION_4_0
#define GL_VERSION_4_0 1
#define GL_SAMPLE_SHADING                 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE       0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_TEXTURE_CUBE_MAP_ARRAY         0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY   0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY         0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW  0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY     0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_DRAW_INDIRECT_BUFFER           0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING   0x8F43
#define GL_GEOMETRY_SHADER_INVOCATIONS    0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS             0x8E71
#define GL_DOUBLE_VEC2                    0x8FFC
#define GL_DOUBLE_VEC3                    0x8FFD
#define GL_DOUBLE_VEC4                    0x8FFE
#define GL_DOUBLE_MAT2                    0x8F46
#define GL_DOUBLE_MAT3                    0x8F47
#define GL_DOUBLE_MAT4                    0x8F48
#define GL_DOUBLE_MAT2x3                  0x8F49
#define GL_DOUBLE_MAT2x4                  0x8F4A
#define GL_DOUBLE_MAT3x2                  0x8F4B
#define GL_DOUBLE_MAT3x4                  0x8F4C
#define GL_DOUBLE_MAT4x2                  0x8F4D
#define GL_DOUBLE_MAT4x3                  0x8F4E
#define GL_ACTIVE_SUBROUTINES             0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS     0x8DE6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH   0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_MAX_SUBROUTINES                0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_NUM_COMPATIBLE_SUBROUTINES     0x8E4A
#define GL_COMPATIBLE_SUBROUTINES         0x8E4B
#define GL_PATCHES                        0x000E
#define GL_PATCH_VERTICES                 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL      0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL      0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES   0x8E75
#define GL_TESS_GEN_MODE                  0x8E76
#define GL_TESS_GEN_SPACING               0x8E77
#define GL_TESS_GEN_VERTEX_ORDER          0x8E78
#define GL_TESS_GEN_POINT_MODE            0x8E79
#define GL_ISOLINES                       0x8E7A
#define GL_FRACTIONAL_ODD                 0x8E7B
#define GL_FRACTIONAL_EVEN                0x8E7C
#define GL_MAX_PATCH_VERTICES             0x8E7D
#define GL_MAX_TESS_GEN_LEVEL             0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS      0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_TRANSFORM_FEEDBACK             0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING     0x8E25
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
typedef void (APIENTRYP PFNGLMINSAMPLESHADINGPROC) (GLfloat value);
typedef void (APIENTRYP PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRYP PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (APIENTRYP PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect);
typedef void (APIENTRYP PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void (APIENTRYP PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void (APIENTRYP PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRYP PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRYP PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble *params);
typedef GLint(APIENTRYP PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar *name);
typedef GLuint(APIENTRYP PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar *name);
typedef void (APIENTRYP PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
typedef void (APIENTRYP PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
typedef void (APIENTRYP PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
typedef void (APIENTRYP PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint *indices);
typedef void (APIENTRYP PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint *values);
typedef void (APIENTRYP PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);
typedef void (APIENTRYP PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat *values);
typedef void (APIENTRYP PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void (APIENTRYP PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint *ids);
typedef void (APIENTRYP PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint *ids);
typedef GLboolean(APIENTRYP PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void (APIENTRYP PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void (APIENTRYP PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void (APIENTRYP PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void (APIENTRYP PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void (APIENTRYP PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMinSampleShading(GLfloat value);
GLAPI void APIENTRY glBlendEquationi(GLuint buf, GLenum mode);
GLAPI void APIENTRY glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLAPI void APIENTRY glBlendFunci(GLuint buf, GLenum src, GLenum dst);
GLAPI void APIENTRY glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLAPI void APIENTRY glDrawArraysIndirect(GLenum mode, const void *indirect);
GLAPI void APIENTRY glDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect);
GLAPI void APIENTRY glUniform1d(GLint location, GLdouble x);
GLAPI void APIENTRY glUniform2d(GLint location, GLdouble x, GLdouble y);
GLAPI void APIENTRY glUniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY glUniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY glUniform1dv(GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glUniform2dv(GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glUniform3dv(GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glUniform4dv(GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glGetUniformdv(GLuint program, GLint location, GLdouble *params);
GLAPI GLint APIENTRY glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar *name);
GLAPI GLuint APIENTRY glGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar *name);
GLAPI void APIENTRY glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
GLAPI void APIENTRY glGetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
GLAPI void APIENTRY glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
GLAPI void APIENTRY glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint *indices);
GLAPI void APIENTRY glGetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint *params);
GLAPI void APIENTRY glGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
GLAPI void APIENTRY glPatchParameteri(GLenum pname, GLint value);
GLAPI void APIENTRY glPatchParameterfv(GLenum pname, const GLfloat *values);
GLAPI void APIENTRY glBindTransformFeedback(GLenum target, GLuint id);
GLAPI void APIENTRY glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids);
GLAPI void APIENTRY glGenTransformFeedbacks(GLsizei n, GLuint *ids);
GLAPI GLboolean APIENTRY glIsTransformFeedback(GLuint id);
GLAPI void APIENTRY glPauseTransformFeedback(void);
GLAPI void APIENTRY glResumeTransformFeedback(void);
GLAPI void APIENTRY glDrawTransformFeedback(GLenum mode, GLuint id);
GLAPI void APIENTRY glDrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream);
GLAPI void APIENTRY glBeginQueryIndexed(GLenum target, GLuint index, GLuint id);
GLAPI void APIENTRY glEndQueryIndexed(GLenum target, GLuint index);
GLAPI void APIENTRY glGetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint *params);
#endif
#endif /* GL_VERSION_4_0 /

#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1
#define GL_FIXED                          0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_RGB565                         0x8D62
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH          0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS     0x87FE
#define GL_PROGRAM_BINARY_FORMATS         0x87FF
#define GL_VERTEX_SHADER_BIT              0x00000001
#define GL_FRAGMENT_SHADER_BIT            0x00000002
#define GL_GEOMETRY_SHADER_BIT            0x00000004
#define GL_TESS_CONTROL_SHADER_BIT        0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT     0x00000010
#define GL_ALL_SHADER_BITS                0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE              0x8258
#define GL_ACTIVE_PROGRAM                 0x8259
#define GL_PROGRAM_PIPELINE_BINDING       0x825A
#define GL_MAX_VIEWPORTS                  0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS         0x825C
#define GL_VIEWPORT_BOUNDS_RANGE          0x825D
#define GL_LAYER_PROVOKING_VERTEX         0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX               0x8260
typedef void (APIENTRYP PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (APIENTRYP PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
typedef void (APIENTRYP PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
typedef void (APIENTRYP PFNGLDEPTHRANGEFPROC) (GLfloat n, GLfloat f);
typedef void (APIENTRYP PFNGLCLEARDEPTHFPROC) (GLfloat d);
typedef void (APIENTRYP PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
typedef void (APIENTRYP PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
typedef void (APIENTRYP PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (APIENTRYP PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef GLuint(APIENTRYP PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar *const*strings);
typedef void (APIENTRYP PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint *pipelines);
typedef void (APIENTRYP PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef GLboolean(APIENTRYP PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint v0);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble v0);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint v0);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRYP PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint *v);
typedef void (APIENTRYP PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint *v);
typedef void (APIENTRYP PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLdouble n, GLdouble f);
typedef void (APIENTRYP PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat *data);
typedef void (APIENTRYP PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble *data);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glReleaseShaderCompiler(void);
GLAPI void APIENTRY glShaderBinary(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
GLAPI void APIENTRY glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
GLAPI void APIENTRY glDepthRangef(GLfloat n, GLfloat f);
GLAPI void APIENTRY glClearDepthf(GLfloat d);
GLAPI void APIENTRY glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
GLAPI void APIENTRY glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
GLAPI void APIENTRY glProgramParameteri(GLuint program, GLenum pname, GLint value);
GLAPI void APIENTRY glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);
GLAPI void APIENTRY glActiveShaderProgram(GLuint pipeline, GLuint program);
GLAPI GLuint APIENTRY glCreateShaderProgramv(GLenum type, GLsizei count, const GLchar *const*strings);
GLAPI void APIENTRY glBindProgramPipeline(GLuint pipeline);
GLAPI void APIENTRY glDeleteProgramPipelines(GLsizei n, const GLuint *pipelines);
GLAPI void APIENTRY glGenProgramPipelines(GLsizei n, GLuint *pipelines);
GLAPI GLboolean APIENTRY glIsProgramPipeline(GLuint pipeline);
GLAPI void APIENTRY glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params);
GLAPI void APIENTRY glProgramUniform1i(GLuint program, GLint location, GLint v0);
GLAPI void APIENTRY glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY glProgramUniform1f(GLuint program, GLint location, GLfloat v0);
GLAPI void APIENTRY glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY glProgramUniform1d(GLuint program, GLint location, GLdouble v0);
GLAPI void APIENTRY glProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glProgramUniform1ui(GLuint program, GLint location, GLuint v0);
GLAPI void APIENTRY glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY glProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1);
GLAPI void APIENTRY glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY glProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1);
GLAPI void APIENTRY glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY glProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1);
GLAPI void APIENTRY glProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1);
GLAPI void APIENTRY glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY glProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
GLAPI void APIENTRY glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY glProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI void APIENTRY glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY glProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
GLAPI void APIENTRY glProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI void APIENTRY glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY glProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI void APIENTRY glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY glProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI void APIENTRY glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY glProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
GLAPI void APIENTRY glProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLAPI void APIENTRY glProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI void APIENTRY glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLAPI void APIENTRY glValidateProgramPipeline(GLuint pipeline);
GLAPI void APIENTRY glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void APIENTRY glVertexAttribL1d(GLuint index, GLdouble x);
GLAPI void APIENTRY glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y);
GLAPI void APIENTRY glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY glVertexAttribL1dv(GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribL2dv(GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribL3dv(GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribL4dv(GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI void APIENTRY glGetVertexAttribLdv(GLuint index, GLenum pname, GLdouble *params);
GLAPI void APIENTRY glViewportArrayv(GLuint first, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
GLAPI void APIENTRY glViewportIndexedfv(GLuint index, const GLfloat *v);
GLAPI void APIENTRY glScissorArrayv(GLuint first, GLsizei count, const GLint *v);
GLAPI void APIENTRY glScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
GLAPI void APIENTRY glScissorIndexedv(GLuint index, const GLint *v);
GLAPI void APIENTRY glDepthRangeArrayv(GLuint first, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glDepthRangeIndexed(GLuint index, GLdouble n, GLdouble f);
GLAPI void APIENTRY glGetFloati_v(GLenum target, GLuint index, GLfloat *data);
GLAPI void APIENTRY glGetDoublei_v(GLenum target, GLuint index, GLdouble *data);
#endif
#endif /* GL_VERSION_4_1 /

#ifndef GL_VERSION_4_2
#define GL_VERSION_4_2 1
#define GL_COPY_READ_BUFFER_BINDING       0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING      0x8F37
#define GL_TRANSFORM_FEEDBACK_ACTIVE      0x8E24
#define GL_TRANSFORM_FEEDBACK_PAUSED      0x8E23
#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH  0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH  0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE   0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH    0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT   0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH    0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE     0x912E
#define GL_NUM_SAMPLE_COUNTS              0x9380
#define GL_MIN_MAP_BUFFER_ALIGNMENT       0x90BC
#define GL_ATOMIC_COUNTER_BUFFER          0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING  0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START    0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE     0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS     0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS   0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS   0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS   0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER    0x92DB
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT      0x00000002
#define GL_UNIFORM_BARRIER_BIT            0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT      0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT            0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT       0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT     0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT      0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT        0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT     0x00001000
#define GL_ALL_BARRIER_BITS               0xFFFFFFFF
#define GL_MAX_IMAGE_UNITS                0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME             0x8F3A
#define GL_IMAGE_BINDING_LEVEL            0x8F3B
#define GL_IMAGE_BINDING_LAYERED          0x8F3C
#define GL_IMAGE_BINDING_LAYER            0x8F3D
#define GL_IMAGE_BINDING_ACCESS           0x8F3E
#define GL_IMAGE_1D                       0x904C
#define GL_IMAGE_2D                       0x904D
#define GL_IMAGE_3D                       0x904E
#define GL_IMAGE_2D_RECT                  0x904F
#define GL_IMAGE_CUBE                     0x9050
#define GL_IMAGE_BUFFER                   0x9051
#define GL_IMAGE_1D_ARRAY                 0x9052
#define GL_IMAGE_2D_ARRAY                 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY           0x9054
#define GL_IMAGE_2D_MULTISAMPLE           0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY     0x9056
#define GL_INT_IMAGE_1D                   0x9057
#define GL_INT_IMAGE_2D                   0x9058
#define GL_INT_IMAGE_3D                   0x9059
#define GL_INT_IMAGE_2D_RECT              0x905A
#define GL_INT_IMAGE_CUBE                 0x905B
#define GL_INT_IMAGE_BUFFER               0x905C
#define GL_INT_IMAGE_1D_ARRAY             0x905D
#define GL_INT_IMAGE_2D_ARRAY             0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY       0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE       0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D          0x9062
#define GL_UNSIGNED_INT_IMAGE_2D          0x9063
#define GL_UNSIGNED_INT_IMAGE_3D          0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT     0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE        0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER      0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY    0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY    0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES              0x906D
#define GL_IMAGE_BINDING_FORMAT           0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS      0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS    0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS    0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS    0x90CF
#define GL_COMPRESSED_RGBA_BPTC_UNORM     0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
typedef void (APIENTRYP PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
typedef void (APIENTRYP PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (APIENTRYP PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);
typedef void (APIENTRYP PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (APIENTRYP PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei instancecount);
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
GLAPI void APIENTRY glDrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
GLAPI void APIENTRY glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
GLAPI void APIENTRY glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
GLAPI void APIENTRY glGetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
GLAPI void APIENTRY glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GLAPI void APIENTRY glMemoryBarrier(GLbitfield barriers);
GLAPI void APIENTRY glTexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GLAPI void APIENTRY glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY glDrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei instancecount);
GLAPI void APIENTRY glDrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
#endif
#endif /* GL_VERSION_4_2 /

#ifndef GL_VERSION_4_3
#define GL_VERSION_4_3 1
typedef void (APIENTRY  *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#define GL_NUM_SHADING_LANGUAGE_VERSIONS  0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG       0x874E
#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_SRGB8_ETC2          0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_COMPRESSED_R11_EAC             0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC      0x9271
#define GL_COMPRESSED_RG11_EAC            0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC     0x9273
#define GL_PRIMITIVE_RESTART_FIXED_INDEX  0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX              0x8D6B
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS     0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS     0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS    0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT   0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE    0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER       0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER_BIT             0x00000020
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION        0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM      0x8245
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH       0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES      0x9144
#define GL_DEBUG_LOGGED_MESSAGES          0x9145
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH    0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH        0x826D
#define GL_BUFFER                         0x82E0
#define GL_SHADER                         0x82E1
#define GL_PROGRAM                        0x82E2
#define GL_QUERY                          0x82E3
#define GL_PROGRAM_PIPELINE               0x82E4
#define GL_SAMPLER                        0x82E6
#define GL_MAX_LABEL_LENGTH               0x82E8
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT         0x00000002
#define GL_MAX_UNIFORM_LOCATIONS          0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH      0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT     0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS     0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES    0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH          0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT         0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS         0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES        0x9318
#define GL_INTERNALFORMAT_SUPPORTED       0x826F
#define GL_INTERNALFORMAT_PREFERRED       0x8270
#define GL_INTERNALFORMAT_RED_SIZE        0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE      0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE       0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE      0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE      0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE    0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE     0x8277
#define GL_INTERNALFORMAT_RED_TYPE        0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE      0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE       0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE      0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE      0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE    0x827D
#define GL_MAX_WIDTH                      0x827E
#define GL_MAX_HEIGHT                     0x827F
#define GL_MAX_DEPTH                      0x8280
#define GL_MAX_LAYERS                     0x8281
#define GL_MAX_COMBINED_DIMENSIONS        0x8282
#define GL_COLOR_COMPONENTS               0x8283
#define GL_DEPTH_COMPONENTS               0x8284
#define GL_STENCIL_COMPONENTS             0x8285
#define GL_COLOR_RENDERABLE               0x8286
#define GL_DEPTH_RENDERABLE               0x8287
#define GL_STENCIL_RENDERABLE             0x8288
#define GL_FRAMEBUFFER_RENDERABLE         0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND              0x828B
#define GL_READ_PIXELS                    0x828C
#define GL_READ_PIXELS_FORMAT             0x828D
#define GL_READ_PIXELS_TYPE               0x828E
#define GL_TEXTURE_IMAGE_FORMAT           0x828F
#define GL_TEXTURE_IMAGE_TYPE             0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT       0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE         0x8292
#define GL_MIPMAP                         0x8293
#define GL_MANUAL_GENERATE_MIPMAP         0x8294
#define GL_AUTO_GENERATE_MIPMAP           0x8295
#define GL_COLOR_ENCODING                 0x8296
#define GL_SRGB_READ                      0x8297
#define GL_SRGB_WRITE                     0x8298
#define GL_FILTER                         0x829A
#define GL_VERTEX_TEXTURE                 0x829B
#define GL_TESS_CONTROL_TEXTURE           0x829C
#define GL_TESS_EVALUATION_TEXTURE        0x829D
#define GL_GEOMETRY_TEXTURE               0x829E
#define GL_FRAGMENT_TEXTURE               0x829F
#define GL_COMPUTE_TEXTURE                0x82A0
#define GL_TEXTURE_SHADOW                 0x82A1
#define GL_TEXTURE_GATHER                 0x82A2
#define GL_TEXTURE_GATHER_SHADOW          0x82A3
#define GL_SHADER_IMAGE_LOAD              0x82A4
#define GL_SHADER_IMAGE_STORE             0x82A5
#define GL_SHADER_IMAGE_ATOMIC            0x82A6
#define GL_IMAGE_TEXEL_SIZE               0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS      0x82A8
#define GL_IMAGE_PIXEL_FORMAT             0x82A9
#define GL_IMAGE_PIXEL_TYPE               0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE  0x82B3
#define GL_CLEAR_BUFFER                   0x82B4
#define GL_TEXTURE_VIEW                   0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS       0x82B6
#define GL_FULL_SUPPORT                   0x82B7
#define GL_CAVEAT_SUPPORT                 0x82B8
#define GL_IMAGE_CLASS_4_X_32             0x82B9
#define GL_IMAGE_CLASS_2_X_32             0x82BA
#define GL_IMAGE_CLASS_1_X_32             0x82BB
#define GL_IMAGE_CLASS_4_X_16             0x82BC
#define GL_IMAGE_CLASS_2_X_16             0x82BD
#define GL_IMAGE_CLASS_1_X_16             0x82BE
#define GL_IMAGE_CLASS_4_X_8              0x82BF
#define GL_IMAGE_CLASS_2_X_8              0x82C0
#define GL_IMAGE_CLASS_1_X_8              0x82C1
#define GL_IMAGE_CLASS_11_11_10           0x82C2
#define GL_IMAGE_CLASS_10_10_10_2         0x82C3
#define GL_VIEW_CLASS_128_BITS            0x82C4
#define GL_VIEW_CLASS_96_BITS             0x82C5
#define GL_VIEW_CLASS_64_BITS             0x82C6
#define GL_VIEW_CLASS_48_BITS             0x82C7
#define GL_VIEW_CLASS_32_BITS             0x82C8
#define GL_VIEW_CLASS_24_BITS             0x82C9
#define GL_VIEW_CLASS_16_BITS             0x82CA
#define GL_VIEW_CLASS_8_BITS              0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB       0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA      0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA      0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA      0x82CF
#define GL_VIEW_CLASS_RGTC1_RED           0x82D0
#define GL_VIEW_CLASS_RGTC2_RG            0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM          0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT          0x82D3
#define GL_UNIFORM                        0x92E1
#define GL_UNIFORM_BLOCK                  0x92E2
#define GL_PROGRAM_INPUT                  0x92E3
#define GL_PROGRAM_OUTPUT                 0x92E4
#define GL_BUFFER_VARIABLE                0x92E5
#define GL_SHADER_STORAGE_BLOCK           0x92E6
#define GL_VERTEX_SUBROUTINE              0x92E8
#define GL_TESS_CONTROL_SUBROUTINE        0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE     0x92EA
#define GL_GEOMETRY_SUBROUTINE            0x92EB
#define GL_FRAGMENT_SUBROUTINE            0x92EC
#define GL_COMPUTE_SUBROUTINE             0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM      0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM    0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM    0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM     0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING     0x92F4
#define GL_ACTIVE_RESOURCES               0x92F5
#define GL_MAX_NAME_LENGTH                0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES       0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH                    0x92F9
#define GL_TYPE                           0x92FA
#define GL_ARRAY_SIZE                     0x92FB
#define GL_OFFSET                         0x92FC
#define GL_BLOCK_INDEX                    0x92FD
#define GL_ARRAY_STRIDE                   0x92FE
#define GL_MATRIX_STRIDE                  0x92FF
#define GL_IS_ROW_MAJOR                   0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX    0x9301
#define GL_BUFFER_BINDING                 0x9302
#define GL_BUFFER_DATA_SIZE               0x9303
#define GL_NUM_ACTIVE_VARIABLES           0x9304
#define GL_ACTIVE_VARIABLES               0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER    0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER  0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER  0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER   0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE           0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE         0x930D
#define GL_LOCATION                       0x930E
#define GL_LOCATION_INDEX                 0x930F
#define GL_IS_PER_PATCH                   0x92E7
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING  0x90D3
#define GL_SHADER_STORAGE_BUFFER_START    0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE     0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE  0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT     0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE     0x90EA
#define GL_TEXTURE_BUFFER_OFFSET          0x919D
#define GL_TEXTURE_BUFFER_SIZE            0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F
#define GL_TEXTURE_VIEW_MIN_LEVEL         0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS        0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER         0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS        0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS       0x82DF
#define GL_VERTEX_ATTRIB_BINDING          0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET  0x82D5
#define GL_VERTEX_BINDING_DIVISOR         0x82D6
#define GL_VERTEX_BINDING_OFFSET          0x82D7
#define GL_VERTEX_BINDING_STRIDE          0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS     0x82DA
#define GL_VERTEX_BINDING_BUFFER          0x8F4F
typedef void (APIENTRYP PFNGLCLEARBUFFERDATAPROC) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (APIENTRYP PFNGLCLEARBUFFERSUBDATAPROC) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void (APIENTRYP PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (APIENTRYP PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);
typedef void (APIENTRYP PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void (APIENTRYP PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETINTERNALFORMATI64VPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
typedef void (APIENTRYP PFNGLINVALIDATETEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
typedef void (APIENTRYP PFNGLINVALIDATETEXIMAGEPROC) (GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLINVALIDATEBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (APIENTRYP PFNGLINVALIDATEBUFFERDATAPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments);
typedef void (APIENTRYP PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void (APIENTRYP PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint *params);
typedef GLuint(APIENTRYP PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (APIENTRYP PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void (APIENTRYP PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
typedef GLint(APIENTRYP PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef GLint(APIENTRYP PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (APIENTRYP PFNGLSHADERSTORAGEBLOCKBINDINGPROC) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
typedef void (APIENTRYP PFNGLTEXBUFFERRANGEPROC) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLTEXSTORAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLTEXTUREVIEWPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
typedef void (APIENTRYP PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXATTRIBLFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (APIENTRYP PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);
typedef void (APIENTRYP PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
typedef void (APIENTRYP PFNGLDEBUGMESSAGEINSERTPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
typedef void (APIENTRYP PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
typedef GLuint(APIENTRYP PFNGLGETDEBUGMESSAGELOGPROC) (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
typedef void (APIENTRYP PFNGLPUSHDEBUGGROUPPROC) (GLenum source, GLuint id, GLsizei length, const GLchar *message);
typedef void (APIENTRYP PFNGLPOPDEBUGGROUPPROC) (void);
typedef void (APIENTRYP PFNGLOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
typedef void (APIENTRYP PFNGLGETOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
typedef void (APIENTRYP PFNGLOBJECTPTRLABELPROC) (const void *ptr, GLsizei length, const GLchar *label);
typedef void (APIENTRYP PFNGLGETOBJECTPTRLABELPROC) (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
GLAPI void APIENTRY glClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
GLAPI void APIENTRY glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GLAPI void APIENTRY glDispatchComputeIndirect(GLintptr indirect);
GLAPI void APIENTRY glCopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GLAPI void APIENTRY glFramebufferParameteri(GLenum target, GLenum pname, GLint param);
GLAPI void APIENTRY glGetFramebufferParameteriv(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
GLAPI void APIENTRY glInvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY glInvalidateTexImage(GLuint texture, GLint level);
GLAPI void APIENTRY glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLAPI void APIENTRY glInvalidateBufferData(GLuint buffer);
GLAPI void APIENTRY glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments);
GLAPI void APIENTRY glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glMultiDrawArraysIndirect(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
GLAPI void APIENTRY glMultiDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
GLAPI void APIENTRY glGetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
GLAPI GLuint APIENTRY glGetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI void APIENTRY glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
GLAPI void APIENTRY glGetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
GLAPI GLint APIENTRY glGetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI GLint APIENTRY glGetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI void APIENTRY glShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
GLAPI void APIENTRY glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI void APIENTRY glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI void APIENTRY glTextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
GLAPI void APIENTRY glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GLAPI void APIENTRY glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GLAPI void APIENTRY glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
GLAPI void APIENTRY glVertexBindingDivisor(GLuint bindingindex, GLuint divisor);
GLAPI void APIENTRY glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI void APIENTRY glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GLAPI void APIENTRY glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam);
GLAPI GLuint APIENTRY glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
GLAPI void APIENTRY glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message);
GLAPI void APIENTRY glPopDebugGroup(void);
GLAPI void APIENTRY glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
GLAPI void APIENTRY glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI void APIENTRY glObjectPtrLabel(const void *ptr, GLsizei length, const GLchar *label);
GLAPI void APIENTRY glGetObjectPtrLabel(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
#endif
#endif /* GL_VERSION_4_3 /

#ifndef GL_VERSION_4_4
#define GL_VERSION_4_4 1
#define GL_MAX_VERTEX_ATTRIB_STRIDE       0x82E5
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_TEXTURE_BUFFER_BINDING         0x8C2A
#define GL_MAP_PERSISTENT_BIT             0x0040
#define GL_MAP_COHERENT_BIT               0x0080
#define GL_DYNAMIC_STORAGE_BIT            0x0100
#define GL_CLIENT_STORAGE_BIT             0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE       0x821F
#define GL_BUFFER_STORAGE_FLAGS           0x8220
#define GL_CLEAR_TEXTURE                  0x9365
#define GL_LOCATION_COMPONENT             0x934A
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C
#define GL_QUERY_BUFFER                   0x9192
#define GL_QUERY_BUFFER_BARRIER_BIT       0x00008000
#define GL_QUERY_BUFFER_BINDING           0x9193
#define GL_QUERY_RESULT_NO_WAIT           0x9194
#define GL_MIRROR_CLAMP_TO_EDGE           0x8743
typedef void (APIENTRYP PFNGLBUFFERSTORAGEPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (APIENTRYP PFNGLCLEARTEXIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
typedef void (APIENTRYP PFNGLCLEARTEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
typedef void (APIENTRYP PFNGLBINDBUFFERSBASEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERSRANGEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
typedef void (APIENTRYP PFNGLBINDTEXTURESPROC) (GLuint first, GLsizei count, const GLuint *textures);
typedef void (APIENTRYP PFNGLBINDSAMPLERSPROC) (GLuint first, GLsizei count, const GLuint *samplers);
typedef void (APIENTRYP PFNGLBINDIMAGETEXTURESPROC) (GLuint first, GLsizei count, const GLuint *textures);
typedef void (APIENTRYP PFNGLBINDVERTEXBUFFERSPROC) (GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
GLAPI void APIENTRY glClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
GLAPI void APIENTRY glClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
GLAPI void APIENTRY glBindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
GLAPI void APIENTRY glBindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
GLAPI void APIENTRY glBindTextures(GLuint first, GLsizei count, const GLuint *textures);
GLAPI void APIENTRY glBindSamplers(GLuint first, GLsizei count, const GLuint *samplers);
GLAPI void APIENTRY glBindImageTextures(GLuint first, GLsizei count, const GLuint *textures);
GLAPI void APIENTRY glBindVertexBuffers(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
#endif
#endif /* GL_VERSION_4_4 /

#ifndef GL_VERSION_4_5
#define GL_VERSION_4_5 1
#define GL_CONTEXT_LOST                   0x0507
#define GL_NEGATIVE_ONE_TO_ONE            0x935E
#define GL_ZERO_TO_ONE                    0x935F
#define GL_CLIP_ORIGIN                    0x935C
#define GL_CLIP_DEPTH_MODE                0x935D
#define GL_QUERY_WAIT_INVERTED            0x8E17
#define GL_QUERY_NO_WAIT_INVERTED         0x8E18
#define GL_QUERY_BY_REGION_WAIT_INVERTED  0x8E19
#define GL_QUERY_BY_REGION_NO_WAIT_INVERTED 0x8E1A
#define GL_MAX_CULL_DISTANCES             0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES 0x82FA
#define GL_TEXTURE_TARGET                 0x1006
#define GL_QUERY_TARGET                   0x82EA
#define GL_GUILTY_CONTEXT_RESET           0x8253
#define GL_INNOCENT_CONTEXT_RESET         0x8254
#define GL_UNKNOWN_CONTEXT_RESET          0x8255
#define GL_RESET_NOTIFICATION_STRATEGY    0x8256
#define GL_LOSE_CONTEXT_ON_RESET          0x8252
#define GL_NO_RESET_NOTIFICATION          0x8261
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT 0x00000004
#define GL_CONTEXT_RELEASE_BEHAVIOR       0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH 0x82FC
typedef void (APIENTRYP PFNGLCLIPCONTROLPROC) (GLenum origin, GLenum depth);
typedef void (APIENTRYP PFNGLCREATETRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) (GLuint xfb, GLuint index, GLuint buffer);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) (GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKIVPROC) (GLuint xfb, GLenum pname, GLint *param);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKI_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint *param);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKI64_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
typedef void (APIENTRYP PFNGLCREATEBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRYP PFNGLNAMEDBUFFERSTORAGEPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (APIENTRYP PFNGLNAMEDBUFFERDATAPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRYP PFNGLNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (APIENTRYP PFNGLCOPYNAMEDBUFFERSUBDATAPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLCLEARNAMEDBUFFERDATAPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (APIENTRYP PFNGLCLEARNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void *(APIENTRYP PFNGLMAPNAMEDBUFFERPROC) (GLuint buffer, GLenum access);
typedef void *(APIENTRYP PFNGLMAPNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean(APIENTRYP PFNGLUNMAPNAMEDBUFFERPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERPARAMETERIVPROC) (GLuint buffer, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERPARAMETERI64VPROC) (GLuint buffer, GLenum pname, GLint64 *params);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERPOINTERVPROC) (GLuint buffer, GLenum pname, void **params);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
typedef void (APIENTRYP PFNGLCREATEFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC) (GLuint framebuffer, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC) (GLuint framebuffer, GLenum buf);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) (GLuint framebuffer, GLsizei n, const GLenum *bufs);
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC) (GLuint framebuffer, GLenum src);
typedef void (APIENTRYP PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
typedef void (APIENTRYP PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLCLEARNAMEDFRAMEBUFFERIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void (APIENTRYP PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void (APIENTRYP PFNGLCLEARNAMEDFRAMEBUFFERFVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void (APIENTRYP PFNGLCLEARNAMEDFRAMEBUFFERFIPROC) (GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil);
typedef void (APIENTRYP PFNGLBLITNAMEDFRAMEBUFFERPROC) (GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum(APIENTRYP PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC) (GLuint framebuffer, GLenum target);
typedef void (APIENTRYP PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC) (GLuint framebuffer, GLenum pname, GLint *param);
typedef void (APIENTRYP PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLCREATERENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLNAMEDRENDERBUFFERSTORAGEPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) (GLuint renderbuffer, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLCREATETEXTURESPROC) (GLenum target, GLsizei n, GLuint *textures);
typedef void (APIENTRYP PFNGLTEXTUREBUFFERPROC) (GLuint texture, GLenum internalformat, GLuint buffer);
typedef void (APIENTRYP PFNGLTEXTUREBUFFERRANGEPROC) (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLTEXTURESTORAGE1DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (APIENTRYP PFNGLTEXTURESTORAGE2DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXTURESTORAGE3DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (APIENTRYP PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRYP PFNGLCOPYTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (APIENTRYP PFNGLCOPYTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLCOPYTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERFPROC) (GLuint texture, GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, const GLfloat *param);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERIPROC) (GLuint texture, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, const GLint *params);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, const GLuint *params);
typedef void (APIENTRYP PFNGLTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, const GLint *param);
typedef void (APIENTRYP PFNGLGENERATETEXTUREMIPMAPPROC) (GLuint texture);
typedef void (APIENTRYP PFNGLBINDTEXTUREUNITPROC) (GLuint unit, GLuint texture);
typedef void (APIENTRYP PFNGLGETTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void (APIENTRYP PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLsizei bufSize, void *pixels);
typedef void (APIENTRYP PFNGLGETTEXTURELEVELPARAMETERFVPROC) (GLuint texture, GLint level, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETTEXTURELEVELPARAMETERIVPROC) (GLuint texture, GLint level, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, GLuint *params);
typedef void (APIENTRYP PFNGLGETTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLCREATEVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRYP PFNGLDISABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void (APIENTRYP PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void (APIENTRYP PFNGLVERTEXARRAYELEMENTBUFFERPROC) (GLuint vaobj, GLuint buffer);
typedef void (APIENTRYP PFNGLVERTEXARRAYVERTEXBUFFERPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXARRAYVERTEXBUFFERSPROC) (GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
typedef void (APIENTRYP PFNGLVERTEXARRAYATTRIBBINDINGPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void (APIENTRYP PFNGLVERTEXARRAYATTRIBFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXARRAYATTRIBIFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXARRAYATTRIBLFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (APIENTRYP PFNGLVERTEXARRAYBINDINGDIVISORPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef void (APIENTRYP PFNGLGETVERTEXARRAYIVPROC) (GLuint vaobj, GLenum pname, GLint *param);
typedef void (APIENTRYP PFNGLGETVERTEXARRAYINDEXEDIVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint *param);
typedef void (APIENTRYP PFNGLGETVERTEXARRAYINDEXED64IVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
typedef void (APIENTRYP PFNGLCREATESAMPLERSPROC) (GLsizei n, GLuint *samplers);
typedef void (APIENTRYP PFNGLCREATEPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef void (APIENTRYP PFNGLCREATEQUERIESPROC) (GLenum target, GLsizei n, GLuint *ids);
typedef void (APIENTRYP PFNGLGETQUERYBUFFEROBJECTI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (APIENTRYP PFNGLGETQUERYBUFFEROBJECTIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (APIENTRYP PFNGLGETQUERYBUFFEROBJECTUI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (APIENTRYP PFNGLGETQUERYBUFFEROBJECTUIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (APIENTRYP PFNGLMEMORYBARRIERBYREGIONPROC) (GLbitfield barriers);
typedef void (APIENTRYP PFNGLGETTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void (APIENTRYP PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
typedef GLenum(APIENTRYP PFNGLGETGRAPHICSRESETSTATUSPROC) (void);
typedef void (APIENTRYP PFNGLGETNCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLsizei bufSize, void *pixels);
typedef void (APIENTRYP PFNGLGETNTEXIMAGEPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void (APIENTRYP PFNGLGETNUNIFORMDVPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMFVPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLint *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMUIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint *params);
typedef void (APIENTRYP PFNGLREADNPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
typedef void (APIENTRYP PFNGLTEXTUREBARRIERPROC) (void);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glClipControl(GLenum origin, GLenum depth);
GLAPI void APIENTRY glCreateTransformFeedbacks(GLsizei n, GLuint *ids);
GLAPI void APIENTRY glTransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer);
GLAPI void APIENTRY glTransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY glGetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint *param);
GLAPI void APIENTRY glGetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint *param);
GLAPI void APIENTRY glGetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
GLAPI void APIENTRY glCreateBuffers(GLsizei n, GLuint *buffers);
GLAPI void APIENTRY glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
GLAPI void APIENTRY glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
GLAPI void APIENTRY glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
GLAPI void APIENTRY glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLAPI void APIENTRY glClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
GLAPI void APIENTRY glClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
GLAPI void *APIENTRY glMapNamedBuffer(GLuint buffer, GLenum access);
GLAPI void *APIENTRY glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI GLboolean APIENTRY glUnmapNamedBuffer(GLuint buffer);
GLAPI void APIENTRY glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLAPI void APIENTRY glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 *params);
GLAPI void APIENTRY glGetNamedBufferPointerv(GLuint buffer, GLenum pname, void **params);
GLAPI void APIENTRY glGetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
GLAPI void APIENTRY glCreateFramebuffers(GLsizei n, GLuint *framebuffers);
GLAPI void APIENTRY glNamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI void APIENTRY glNamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param);
GLAPI void APIENTRY glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
GLAPI void APIENTRY glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI void APIENTRY glNamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf);
GLAPI void APIENTRY glNamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum *bufs);
GLAPI void APIENTRY glNamedFramebufferReadBuffer(GLuint framebuffer, GLenum src);
GLAPI void APIENTRY glInvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
GLAPI void APIENTRY glInvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
GLAPI void APIENTRY glClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
GLAPI void APIENTRY glClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
GLAPI void APIENTRY glClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil);
GLAPI void APIENTRY glBlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI GLenum APIENTRY glCheckNamedFramebufferStatus(GLuint framebuffer, GLenum target);
GLAPI void APIENTRY glGetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint *param);
GLAPI void APIENTRY glGetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
GLAPI void APIENTRY glCreateRenderbuffers(GLsizei n, GLuint *renderbuffers);
GLAPI void APIENTRY glNamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glNamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glGetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint *params);
GLAPI void APIENTRY glCreateTextures(GLenum target, GLsizei n, GLuint *textures);
GLAPI void APIENTRY glTextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer);
GLAPI void APIENTRY glTextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY glTextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
GLAPI void APIENTRY glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glTextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY glTextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI void APIENTRY glTextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI void APIENTRY glTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
GLAPI void APIENTRY glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI void APIENTRY glTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
GLAPI void APIENTRY glCompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
GLAPI void APIENTRY glCompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
GLAPI void APIENTRY glCompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
GLAPI void APIENTRY glCopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY glCopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glCopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glTextureParameterf(GLuint texture, GLenum pname, GLfloat param);
GLAPI void APIENTRY glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat *param);
GLAPI void APIENTRY glTextureParameteri(GLuint texture, GLenum pname, GLint param);
GLAPI void APIENTRY glTextureParameterIiv(GLuint texture, GLenum pname, const GLint *params);
GLAPI void APIENTRY glTextureParameterIuiv(GLuint texture, GLenum pname, const GLuint *params);
GLAPI void APIENTRY glTextureParameteriv(GLuint texture, GLenum pname, const GLint *param);
GLAPI void APIENTRY glGenerateTextureMipmap(GLuint texture);
GLAPI void APIENTRY glBindTextureUnit(GLuint unit, GLuint texture);
GLAPI void APIENTRY glGetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
GLAPI void APIENTRY glGetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void *pixels);
GLAPI void APIENTRY glGetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetTextureParameterfv(GLuint texture, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetTextureParameterIiv(GLuint texture, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint *params);
GLAPI void APIENTRY glGetTextureParameteriv(GLuint texture, GLenum pname, GLint *params);
GLAPI void APIENTRY glCreateVertexArrays(GLsizei n, GLuint *arrays);
GLAPI void APIENTRY glDisableVertexArrayAttrib(GLuint vaobj, GLuint index);
GLAPI void APIENTRY glEnableVertexArrayAttrib(GLuint vaobj, GLuint index);
GLAPI void APIENTRY glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer);
GLAPI void APIENTRY glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GLAPI void APIENTRY glVertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
GLAPI void APIENTRY glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
GLAPI void APIENTRY glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GLAPI void APIENTRY glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY glVertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor);
GLAPI void APIENTRY glGetVertexArrayiv(GLuint vaobj, GLenum pname, GLint *param);
GLAPI void APIENTRY glGetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint *param);
GLAPI void APIENTRY glGetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
GLAPI void APIENTRY glCreateSamplers(GLsizei n, GLuint *samplers);
GLAPI void APIENTRY glCreateProgramPipelines(GLsizei n, GLuint *pipelines);
GLAPI void APIENTRY glCreateQueries(GLenum target, GLsizei n, GLuint *ids);
GLAPI void APIENTRY glGetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
GLAPI void APIENTRY glGetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
GLAPI void APIENTRY glGetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
GLAPI void APIENTRY glGetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
GLAPI void APIENTRY glMemoryBarrierByRegion(GLbitfield barriers);
GLAPI void APIENTRY glGetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
GLAPI void APIENTRY glGetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
GLAPI GLenum APIENTRY glGetGraphicsResetStatus(void);
GLAPI void APIENTRY glGetnCompressedTexImage(GLenum target, GLint lod, GLsizei bufSize, void *pixels);
GLAPI void APIENTRY glGetnTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
GLAPI void APIENTRY glGetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
GLAPI void APIENTRY glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI void APIENTRY glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI void APIENTRY glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI void APIENTRY glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GLAPI void APIENTRY glTextureBarrier(void);
#endif
#endif /* GL_VERSION_4_5 /

#ifndef GL_ARB_ES2_compatibility
#define GL_ARB_ES2_compatibility 1
#endif /* GL_ARB_ES2_compatibility /

#ifndef GL_ARB_ES3_1_compatibility
#define GL_ARB_ES3_1_compatibility 1
#endif /* GL_ARB_ES3_1_compatibility /

#ifndef GL_ARB_ES3_compatibility
#define GL_ARB_ES3_compatibility 1
#endif /* GL_ARB_ES3_compatibility /

#ifndef GL_ARB_arrays_of_arrays
#define GL_ARB_arrays_of_arrays 1
#endif /* GL_ARB_arrays_of_arrays /

#ifndef GL_ARB_base_instance
#define GL_ARB_base_instance 1
#endif /* GL_ARB_base_instance /

#ifndef GL_ARB_bindless_texture
#define GL_ARB_bindless_texture 1
typedef uint64_t GLuint64EXT;
#define GL_UNSIGNED_INT64_ARB             0x140F
typedef GLuint64(APIENTRYP PFNGLGETTEXTUREHANDLEARBPROC) (GLuint texture);
typedef GLuint64(APIENTRYP PFNGLGETTEXTURESAMPLERHANDLEARBPROC) (GLuint texture, GLuint sampler);
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC) (GLuint64 handle);
typedef GLuint64(APIENTRYP PFNGLGETIMAGEHANDLEARBPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLERESIDENTARBPROC) (GLuint64 handle, GLenum access);
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC) (GLuint64 handle);
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64ARBPROC) (GLint location, GLuint64 value);
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64VARBPROC) (GLint location, GLsizei count, const GLuint64 *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC) (GLuint program, GLint location, GLuint64 value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64 *values);
typedef GLboolean(APIENTRYP PFNGLISTEXTUREHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef GLboolean(APIENTRYP PFNGLISIMAGEHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1UI64ARBPROC) (GLuint index, GLuint64EXT x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1UI64VARBPROC) (GLuint index, const GLuint64EXT *v);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBLUI64VARBPROC) (GLuint index, GLenum pname, GLuint64EXT *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLuint64 APIENTRY glGetTextureHandleARB(GLuint texture);
GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleARB(GLuint texture, GLuint sampler);
GLAPI void APIENTRY glMakeTextureHandleResidentARB(GLuint64 handle);
GLAPI void APIENTRY glMakeTextureHandleNonResidentARB(GLuint64 handle);
GLAPI GLuint64 APIENTRY glGetImageHandleARB(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
GLAPI void APIENTRY glMakeImageHandleResidentARB(GLuint64 handle, GLenum access);
GLAPI void APIENTRY glMakeImageHandleNonResidentARB(GLuint64 handle);
GLAPI void APIENTRY glUniformHandleui64ARB(GLint location, GLuint64 value);
GLAPI void APIENTRY glUniformHandleui64vARB(GLint location, GLsizei count, const GLuint64 *value);
GLAPI void APIENTRY glProgramUniformHandleui64ARB(GLuint program, GLint location, GLuint64 value);
GLAPI void APIENTRY glProgramUniformHandleui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *values);
GLAPI GLboolean APIENTRY glIsTextureHandleResidentARB(GLuint64 handle);
GLAPI GLboolean APIENTRY glIsImageHandleResidentARB(GLuint64 handle);
GLAPI void APIENTRY glVertexAttribL1ui64ARB(GLuint index, GLuint64EXT x);
GLAPI void APIENTRY glVertexAttribL1ui64vARB(GLuint index, const GLuint64EXT *v);
GLAPI void APIENTRY glGetVertexAttribLui64vARB(GLuint index, GLenum pname, GLuint64EXT *params);
#endif
#endif /* GL_ARB_bindless_texture /

#ifndef GL_ARB_blend_func_extended
#define GL_ARB_blend_func_extended 1
#endif /* GL_ARB_blend_func_extended /

#ifndef GL_ARB_buffer_storage
#define GL_ARB_buffer_storage 1
#endif /* GL_ARB_buffer_storage /

#ifndef GL_ARB_cl_event
#define GL_ARB_cl_event 1
struct _cl_context;
struct _cl_event;
#define GL_SYNC_CL_EVENT_ARB              0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB     0x8241
typedef GLsync(APIENTRYP PFNGLCREATESYNCFROMCLEVENTARBPROC) (struct _cl_context *context, struct _cl_event *event, GLbitfield flags);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLsync APIENTRY glCreateSyncFromCLeventARB(struct _cl_context *context, struct _cl_event *event, GLbitfield flags);
#endif
#endif /* GL_ARB_cl_event /

#ifndef GL_ARB_clear_buffer_object
#define GL_ARB_clear_buffer_object 1
#endif /* GL_ARB_clear_buffer_object /

#ifndef GL_ARB_clear_texture
#define GL_ARB_clear_texture 1
#endif /* GL_ARB_clear_texture /

#ifndef GL_ARB_clip_control
#define GL_ARB_clip_control 1
#endif /* GL_ARB_clip_control /

#ifndef GL_ARB_compressed_texture_pixel_storage
#define GL_ARB_compressed_texture_pixel_storage 1
#endif /* GL_ARB_compressed_texture_pixel_storage /

#ifndef GL_ARB_compute_shader
#define GL_ARB_compute_shader 1
#endif /* GL_ARB_compute_shader /

#ifndef GL_ARB_compute_variable_group_size
#define GL_ARB_compute_variable_group_size 1
#define GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB 0x9344
#define GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB 0x90EB
#define GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB 0x9345
#define GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB 0x91BF
typedef void (APIENTRYP PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
#endif
#endif /* GL_ARB_compute_variable_group_size /

#ifndef GL_ARB_conditional_render_inverted
#define GL_ARB_conditional_render_inverted 1
#endif /* GL_ARB_conditional_render_inverted /

#ifndef GL_ARB_conservative_depth
#define GL_ARB_conservative_depth 1
#endif /* GL_ARB_conservative_depth /

#ifndef GL_ARB_copy_buffer
#define GL_ARB_copy_buffer 1
#endif /* GL_ARB_copy_buffer /

#ifndef GL_ARB_copy_image
#define GL_ARB_copy_image 1
#endif /* GL_ARB_copy_image /

#ifndef GL_ARB_cull_distance
#define GL_ARB_cull_distance 1
#endif /* GL_ARB_cull_distance /

#ifndef GL_ARB_debug_output
#define GL_ARB_debug_output 1
typedef void (APIENTRY  *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB   0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB    0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB  0x8245
#define GL_DEBUG_SOURCE_API_ARB           0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB   0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB   0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB         0x824B
#define GL_DEBUG_TYPE_ERROR_ARB           0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB     0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB     0x8250
#define GL_DEBUG_TYPE_OTHER_ARB           0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB   0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB  0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB      0x9145
#define GL_DEBUG_SEVERITY_HIGH_ARB        0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB      0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB         0x9148
typedef void (APIENTRYP PFNGLDEBUGMESSAGECONTROLARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
typedef void (APIENTRYP PFNGLDEBUGMESSAGEINSERTARBPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
typedef void (APIENTRYP PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, const void *userParam);
typedef GLuint(APIENTRYP PFNGLGETDEBUGMESSAGELOGARBPROC) (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDebugMessageControlARB(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI void APIENTRY glDebugMessageInsertARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GLAPI void APIENTRY glDebugMessageCallbackARB(GLDEBUGPROCARB callback, const void *userParam);
GLAPI GLuint APIENTRY glGetDebugMessageLogARB(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
#endif
#endif /* GL_ARB_debug_output /

#ifndef GL_ARB_depth_buffer_float
#define GL_ARB_depth_buffer_float 1
#endif /* GL_ARB_depth_buffer_float /

#ifndef GL_ARB_depth_clamp
#define GL_ARB_depth_clamp 1
#endif /* GL_ARB_depth_clamp /

#ifndef GL_ARB_derivative_control
#define GL_ARB_derivative_control 1
#endif /* GL_ARB_derivative_control /

#ifndef GL_ARB_direct_state_access
#define GL_ARB_direct_state_access 1
#endif /* GL_ARB_direct_state_access /

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1
typedef void (APIENTRYP PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRYP PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBlendEquationiARB(GLuint buf, GLenum mode);
GLAPI void APIENTRY glBlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLAPI void APIENTRY glBlendFunciARB(GLuint buf, GLenum src, GLenum dst);
GLAPI void APIENTRY glBlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
#endif
#endif /* GL_ARB_draw_buffers_blend /

#ifndef GL_ARB_draw_elements_base_vertex
#define GL_ARB_draw_elements_base_vertex 1
#endif /* GL_ARB_draw_elements_base_vertex /

#ifndef GL_ARB_draw_indirect
#define GL_ARB_draw_indirect 1
#endif /* GL_ARB_draw_indirect /

#ifndef GL_ARB_enhanced_layouts
#define GL_ARB_enhanced_layouts 1
#endif /* GL_ARB_enhanced_layouts /

#ifndef GL_ARB_explicit_attrib_location
#define GL_ARB_explicit_attrib_location 1
#endif /* GL_ARB_explicit_attrib_location /

#ifndef GL_ARB_explicit_uniform_location
#define GL_ARB_explicit_uniform_location 1
#endif /* GL_ARB_explicit_uniform_location /

#ifndef GL_ARB_fragment_coord_conventions
#define GL_ARB_fragment_coord_conventions 1
#endif /* GL_ARB_fragment_coord_conventions /

#ifndef GL_ARB_fragment_layer_viewport
#define GL_ARB_fragment_layer_viewport 1
#endif /* GL_ARB_fragment_layer_viewport /

#ifndef GL_ARB_framebuffer_no_attachments
#define GL_ARB_framebuffer_no_attachments 1
#endif /* GL_ARB_framebuffer_no_attachments /

#ifndef GL_ARB_framebuffer_object
#define GL_ARB_framebuffer_object 1
#endif /* GL_ARB_framebuffer_object /

#ifndef GL_ARB_framebuffer_sRGB
#define GL_ARB_framebuffer_sRGB 1
#endif /* GL_ARB_framebuffer_sRGB /

#ifndef GL_ARB_get_program_binary
#define GL_ARB_get_program_binary 1
#endif /* GL_ARB_get_program_binary /

#ifndef GL_ARB_get_texture_sub_image
#define GL_ARB_get_texture_sub_image 1
#endif /* GL_ARB_get_texture_sub_image /

#ifndef GL_ARB_gpu_shader5
#define GL_ARB_gpu_shader5 1
#endif /* GL_ARB_gpu_shader5 /

#ifndef GL_ARB_gpu_shader_fp64
#define GL_ARB_gpu_shader_fp64 1
#endif /* GL_ARB_gpu_shader_fp64 /

#ifndef GL_ARB_half_float_vertex
#define GL_ARB_half_float_vertex 1
#endif /* GL_ARB_half_float_vertex /

#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1
#define GL_BLEND_COLOR                    0x8005
#define GL_BLEND_EQUATION                 0x8009
#endif /* GL_ARB_imaging /

#ifndef GL_ARB_indirect_parameters
#define GL_ARB_indirect_parameters 1
#define GL_PARAMETER_BUFFER_ARB           0x80EE
#define GL_PARAMETER_BUFFER_BINDING_ARB   0x80EF
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC) (GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC) (GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMultiDrawArraysIndirectCountARB(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
GLAPI void APIENTRY glMultiDrawElementsIndirectCountARB(GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
#endif
#endif /* GL_ARB_indirect_parameters /

#ifndef GL_ARB_internalformat_query
#define GL_ARB_internalformat_query 1
#endif /* GL_ARB_internalformat_query /

#ifndef GL_ARB_internalformat_query2
#define GL_ARB_internalformat_query2 1
#define GL_SRGB_DECODE_ARB                0x8299
#endif /* GL_ARB_internalformat_query2 /

#ifndef GL_ARB_invalidate_subdata
#define GL_ARB_invalidate_subdata 1
#endif /* GL_ARB_invalidate_subdata /

#ifndef GL_ARB_map_buffer_alignment
#define GL_ARB_map_buffer_alignment 1
#endif /* GL_ARB_map_buffer_alignment /

#ifndef GL_ARB_map_buffer_range
#define GL_ARB_map_buffer_range 1
#endif /* GL_ARB_map_buffer_range /

#ifndef GL_ARB_multi_bind
#define GL_ARB_multi_bind 1
#endif /* GL_ARB_multi_bind /

#ifndef GL_ARB_multi_draw_indirect
#define GL_ARB_multi_draw_indirect 1
#endif /* GL_ARB_multi_draw_indirect /

#ifndef GL_ARB_occlusion_query2
#define GL_ARB_occlusion_query2 1
#endif /* GL_ARB_occlusion_query2 /

#ifndef GL_ARB_pipeline_statistics_query
#define GL_ARB_pipeline_statistics_query 1
#define GL_VERTICES_SUBMITTED_ARB         0x82EE
#define GL_PRIMITIVES_SUBMITTED_ARB       0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS_ARB  0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES_ARB 0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB 0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB 0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS_ARB 0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS_ARB 0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES_ARB  0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES_ARB 0x82F7
#endif /* GL_ARB_pipeline_statistics_query /

#ifndef GL_ARB_program_interface_query
#define GL_ARB_program_interface_query 1
#endif /* GL_ARB_program_interface_query /

#ifndef GL_ARB_provoking_vertex
#define GL_ARB_provoking_vertex 1
#endif /* GL_ARB_provoking_vertex /

#ifndef GL_ARB_query_buffer_object
#define GL_ARB_query_buffer_object 1
#endif /* GL_ARB_query_buffer_object /

#ifndef GL_ARB_robust_buffer_access_behavior
#define GL_ARB_robust_buffer_access_behavior 1
#endif /* GL_ARB_robust_buffer_access_behavior /

#ifndef GL_ARB_robustness
#define GL_ARB_robustness 1
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GL_LOSE_CONTEXT_ON_RESET_ARB      0x8252
#define GL_GUILTY_CONTEXT_RESET_ARB       0x8253
#define GL_INNOCENT_CONTEXT_RESET_ARB     0x8254
#define GL_UNKNOWN_CONTEXT_RESET_ARB      0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GL_NO_RESET_NOTIFICATION_ARB      0x8261
typedef GLenum(APIENTRYP PFNGLGETGRAPHICSRESETSTATUSARBPROC) (void);
typedef void (APIENTRYP PFNGLGETNTEXIMAGEARBPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *img);
typedef void (APIENTRYP PFNGLREADNPIXELSARBPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
typedef void (APIENTRYP PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, GLsizei bufSize, void *img);
typedef void (APIENTRYP PFNGLGETNUNIFORMFVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMUIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint *params);
typedef void (APIENTRYP PFNGLGETNUNIFORMDVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLenum APIENTRY glGetGraphicsResetStatusARB(void);
GLAPI void APIENTRY glGetnTexImageARB(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *img);
GLAPI void APIENTRY glReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GLAPI void APIENTRY glGetnCompressedTexImageARB(GLenum target, GLint lod, GLsizei bufSize, void *img);
GLAPI void APIENTRY glGetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI void APIENTRY glGetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI void APIENTRY glGetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI void APIENTRY glGetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
#endif
#endif /* GL_ARB_robustness /

#ifndef GL_ARB_robustness_isolation
#define GL_ARB_robustness_isolation 1
#endif /* GL_ARB_robustness_isolation /

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1
#define GL_SAMPLE_SHADING_ARB             0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB   0x8C37
typedef void (APIENTRYP PFNGLMINSAMPLESHADINGARBPROC) (GLfloat value);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMinSampleShadingARB(GLfloat value);
#endif
#endif /* GL_ARB_sample_shading /

#ifndef GL_ARB_sampler_objects
#define GL_ARB_sampler_objects 1
#endif /* GL_ARB_sampler_objects /

#ifndef GL_ARB_seamless_cube_map
#define GL_ARB_seamless_cube_map 1
#endif /* GL_ARB_seamless_cube_map /

#ifndef GL_ARB_seamless_cubemap_per_texture
#define GL_ARB_seamless_cubemap_per_texture 1
#endif /* GL_ARB_seamless_cubemap_per_texture /

#ifndef GL_ARB_separate_shader_objects
#define GL_ARB_separate_shader_objects 1
#endif /* GL_ARB_separate_shader_objects /

#ifndef GL_ARB_shader_atomic_counters
#define GL_ARB_shader_atomic_counters 1
#endif /* GL_ARB_shader_atomic_counters /

#ifndef GL_ARB_shader_bit_encoding
#define GL_ARB_shader_bit_encoding 1
#endif /* GL_ARB_shader_bit_encoding /

#ifndef GL_ARB_shader_draw_parameters
#define GL_ARB_shader_draw_parameters 1
#endif /* GL_ARB_shader_draw_parameters /

#ifndef GL_ARB_shader_group_vote
#define GL_ARB_shader_group_vote 1
#endif /* GL_ARB_shader_group_vote /

#ifndef GL_ARB_shader_image_load_store
#define GL_ARB_shader_image_load_store 1
#endif /* GL_ARB_shader_image_load_store /

#ifndef GL_ARB_shader_image_size
#define GL_ARB_shader_image_size 1
#endif /* GL_ARB_shader_image_size /

#ifndef GL_ARB_shader_precision
#define GL_ARB_shader_precision 1
#endif /* GL_ARB_shader_precision /

#ifndef GL_ARB_shader_stencil_export
#define GL_ARB_shader_stencil_export 1
#endif /* GL_ARB_shader_stencil_export /

#ifndef GL_ARB_shader_storage_buffer_object
#define GL_ARB_shader_storage_buffer_object 1
#endif /* GL_ARB_shader_storage_buffer_object /

#ifndef GL_ARB_shader_subroutine
#define GL_ARB_shader_subroutine 1
#endif /* GL_ARB_shader_subroutine /

#ifndef GL_ARB_shader_texture_image_samples
#define GL_ARB_shader_texture_image_samples 1
#endif /* GL_ARB_shader_texture_image_samples /

#ifndef GL_ARB_shading_language_420pack
#define GL_ARB_shading_language_420pack 1
#endif /* GL_ARB_shading_language_420pack /

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1
#define GL_SHADER_INCLUDE_ARB             0x8DAE
#define GL_NAMED_STRING_LENGTH_ARB        0x8DE9
#define GL_NAMED_STRING_TYPE_ARB          0x8DEA
typedef void (APIENTRYP PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
typedef void (APIENTRYP PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name);
typedef void (APIENTRYP PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const GLchar *const*path, const GLint *length);
typedef GLboolean(APIENTRYP PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name);
typedef void (APIENTRYP PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
typedef void (APIENTRYP PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const GLchar *name, GLenum pname, GLint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glNamedStringARB(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
GLAPI void APIENTRY glDeleteNamedStringARB(GLint namelen, const GLchar *name);
GLAPI void APIENTRY glCompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar *const*path, const GLint *length);
GLAPI GLboolean APIENTRY glIsNamedStringARB(GLint namelen, const GLchar *name);
GLAPI void APIENTRY glGetNamedStringARB(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
GLAPI void APIENTRY glGetNamedStringivARB(GLint namelen, const GLchar *name, GLenum pname, GLint *params);
#endif
#endif /* GL_ARB_shading_language_include /

#ifndef GL_ARB_shading_language_packing
#define GL_ARB_shading_language_packing 1
#endif /* GL_ARB_shading_language_packing /

#ifndef GL_ARB_sparse_buffer
#define GL_ARB_sparse_buffer 1
#define GL_SPARSE_STORAGE_BIT_ARB         0x0400
#define GL_SPARSE_BUFFER_PAGE_SIZE_ARB    0x82F8
typedef void (APIENTRYP PFNGLBUFFERPAGECOMMITMENTARBPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
typedef void (APIENTRYP PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
typedef void (APIENTRYP PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBufferPageCommitmentARB(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
GLAPI void APIENTRY glNamedBufferPageCommitmentEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
GLAPI void APIENTRY glNamedBufferPageCommitmentARB(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
#endif
#endif /* GL_ARB_sparse_buffer /

#ifndef GL_ARB_sparse_texture
#define GL_ARB_sparse_texture 1
#define GL_TEXTURE_SPARSE_ARB             0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_ARB    0x91A7
#define GL_NUM_SPARSE_LEVELS_ARB          0x91AA
#define GL_NUM_VIRTUAL_PAGE_SIZES_ARB     0x91A8
#define GL_VIRTUAL_PAGE_SIZE_X_ARB        0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_ARB        0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_ARB        0x9197
#define GL_MAX_SPARSE_TEXTURE_SIZE_ARB    0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB 0x919A
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB 0x91A9
typedef void (APIENTRYP PFNGLTEXPAGECOMMITMENTARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glTexPageCommitmentARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
#endif
#endif /* GL_ARB_sparse_texture /

#ifndef GL_ARB_stencil_texturing
#define GL_ARB_stencil_texturing 1
#endif /* GL_ARB_stencil_texturing /

#ifndef GL_ARB_sync
#define GL_ARB_sync 1
#endif /* GL_ARB_sync /

#ifndef GL_ARB_tessellation_shader
#define GL_ARB_tessellation_shader 1
#endif /* GL_ARB_tessellation_shader /

#ifndef GL_ARB_texture_barrier
#define GL_ARB_texture_barrier 1
#endif /* GL_ARB_texture_barrier /

#ifndef GL_ARB_texture_buffer_object_rgb32
#define GL_ARB_texture_buffer_object_rgb32 1
#endif /* GL_ARB_texture_buffer_object_rgb32 /

#ifndef GL_ARB_texture_buffer_range
#define GL_ARB_texture_buffer_range 1
#endif /* GL_ARB_texture_buffer_range /

#ifndef GL_ARB_texture_compression_bptc
#define GL_ARB_texture_compression_bptc 1
#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F
#endif /* GL_ARB_texture_compression_bptc /

#ifndef GL_ARB_texture_compression_rgtc
#define GL_ARB_texture_compression_rgtc 1
#endif /* GL_ARB_texture_compression_rgtc /

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1
#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB     0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB     0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F
#endif /* GL_ARB_texture_cube_map_array /

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F
#endif /* GL_ARB_texture_gather /

#ifndef GL_ARB_texture_mirror_clamp_to_edge
#define GL_ARB_texture_mirror_clamp_to_edge 1
#endif /* GL_ARB_texture_mirror_clamp_to_edge /

#ifndef GL_ARB_texture_multisample
#define GL_ARB_texture_multisample 1
#endif /* GL_ARB_texture_multisample /

#ifndef GL_ARB_texture_query_levels
#define GL_ARB_texture_query_levels 1
#endif /* GL_ARB_texture_query_levels /

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1
#endif /* GL_ARB_texture_query_lod /

#ifndef GL_ARB_texture_rg
#define GL_ARB_texture_rg 1
#endif /* GL_ARB_texture_rg /

#ifndef GL_ARB_texture_rgb10_a2ui
#define GL_ARB_texture_rgb10_a2ui 1
#endif /* GL_ARB_texture_rgb10_a2ui /

#ifndef GL_ARB_texture_stencil8
#define GL_ARB_texture_stencil8 1
#endif /* GL_ARB_texture_stencil8 /

#ifndef GL_ARB_texture_storage
#define GL_ARB_texture_storage 1
#endif /* GL_ARB_texture_storage /

#ifndef GL_ARB_texture_storage_multisample
#define GL_ARB_texture_storage_multisample 1
#endif /* GL_ARB_texture_storage_multisample /

#ifndef GL_ARB_texture_swizzle
#define GL_ARB_texture_swizzle 1
#endif /* GL_ARB_texture_swizzle /

#ifndef GL_ARB_texture_view
#define GL_ARB_texture_view 1
#endif /* GL_ARB_texture_view /

#ifndef GL_ARB_timer_query
#define GL_ARB_timer_query 1
#endif /* GL_ARB_timer_query /

#ifndef GL_ARB_transform_feedback2
#define GL_ARB_transform_feedback2 1
#endif /* GL_ARB_transform_feedback2 /

#ifndef GL_ARB_transform_feedback3
#define GL_ARB_transform_feedback3 1
#endif /* GL_ARB_transform_feedback3 /

#ifndef GL_ARB_transform_feedback_instanced
#define GL_ARB_transform_feedback_instanced 1
#endif /* GL_ARB_transform_feedback_instanced /

#ifndef GL_ARB_transform_feedback_overflow_query
#define GL_ARB_transform_feedback_overflow_query 1
#define GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB 0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB 0x82ED
#endif /* GL_ARB_transform_feedback_overflow_query /

#ifndef GL_ARB_uniform_buffer_object
#define GL_ARB_uniform_buffer_object 1
#endif /* GL_ARB_uniform_buffer_object /

#ifndef GL_ARB_vertex_array_bgra
#define GL_ARB_vertex_array_bgra 1
#endif /* GL_ARB_vertex_array_bgra /

#ifndef GL_ARB_vertex_array_object
#define GL_ARB_vertex_array_object 1
#endif /* GL_ARB_vertex_array_object /

#ifndef GL_ARB_vertex_attrib_64bit
#define GL_ARB_vertex_attrib_64bit 1
#endif /* GL_ARB_vertex_attrib_64bit /

#ifndef GL_ARB_vertex_attrib_binding
#define GL_ARB_vertex_attrib_binding 1
#endif /* GL_ARB_vertex_attrib_binding /

#ifndef GL_ARB_vertex_type_10f_11f_11f_rev
#define GL_ARB_vertex_type_10f_11f_11f_rev 1
#endif /* GL_ARB_vertex_type_10f_11f_11f_rev /

#ifndef GL_ARB_vertex_type_2_10_10_10_rev
#define GL_ARB_vertex_type_2_10_10_10_rev 1
#endif /* GL_ARB_vertex_type_2_10_10_10_rev /

#ifndef GL_ARB_viewport_array
#define GL_ARB_viewport_array 1
#endif /* GL_ARB_viewport_array /

#ifndef GL_KHR_context_flush_control
#define GL_KHR_context_flush_control 1
#endif /* GL_KHR_context_flush_control /

#ifndef GL_KHR_debug
#define GL_KHR_debug 1
#endif /* GL_KHR_debug /

#ifndef GL_KHR_no_error
#define GL_KHR_no_error 1
#define GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR  0x00000008
#endif /* GL_KHR_no_error /

#ifndef GL_KHR_robust_buffer_access_behavior
#define GL_KHR_robust_buffer_access_behavior 1
#endif /* GL_KHR_robust_buffer_access_behavior /

#ifndef GL_KHR_robustness
#define GL_KHR_robustness 1
#define GL_CONTEXT_ROBUST_ACCESS          0x90F3
#endif /* GL_KHR_robustness /

#ifndef GL_KHR_texture_compression_astc_hdr
#define GL_KHR_texture_compression_astc_hdr 1
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR   0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR   0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR   0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR   0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR   0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR   0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR   0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR   0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR  0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR  0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR  0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD
#endif /* GL_KHR_texture_compression_astc_hdr /

#ifndef GL_KHR_texture_compression_astc_ldr
#define GL_KHR_texture_compression_astc_ldr 1
#endif /* GL_KHR_texture_compression_astc_ldr /

#ifdef __cplusplus
}
#endif

#endif*/
#endif //GL_H
