/* Copyright (c) 2001, Stanford University
	All rights reserved.

	See the file LICENSE.txt for information on redistributing this software. */
	

#include "cr_spu.h"
#include "chromium.h"
#include "cr_error.h"
#include "cr_mem.h"
#include "cr_net.h"
#include "server_dispatch.h"
#include "server.h"

static unsigned int __lookupComponents( GLenum pname )
{
	switch( pname )
	{

			case GL_AMBIENT: return 4;
			case GL_COLOR_INDEXES: return 3;
			case GL_CONSTANT_ATTENUATION: return 1;
			case GL_DIFFUSE: return 4;
			case GL_EMISSION: return 4;
			case GL_EYE_PLANE: return 4;
			case GL_LINEAR_ATTENUATION: return 1;
			case GL_OBJECT_PLANE: return 4;
			case GL_POSITION: return 4;
			case GL_QUADRATIC_ATTENUATION: return 1;
			case GL_SHININESS: return 1;
			case GL_SPECULAR: return 4;
			case GL_SPOT_CUTOFF: return 1;
			case GL_SPOT_DIRECTION: return 3;
			case GL_SPOT_EXPONENT: return 1;
			case GL_TEXTURE_ALPHA_SIZE: return 1;
			case GL_TEXTURE_BLUE_SIZE: return 1;
			case GL_TEXTURE_BORDER: return 1;
			case GL_TEXTURE_BORDER_COLOR: return 4;
			case GL_TEXTURE_COMPONENTS: return 1;
			case GL_TEXTURE_ENV_COLOR: return 4;
			case GL_TEXTURE_ENV_MODE: return 1;
			case GL_TEXTURE_GEN_MODE: return 1;
			case GL_TEXTURE_GREEN_SIZE: return 1;
			case GL_TEXTURE_HEIGHT: return 1;
			case GL_TEXTURE_INTENSITY_SIZE: return 1;
			case GL_TEXTURE_LUMINANCE_SIZE: return 1;
			case GL_TEXTURE_MAG_FILTER: return 1;
			case GL_TEXTURE_MIN_FILTER: return 1;
			case GL_TEXTURE_RED_SIZE: return 1;
			case GL_TEXTURE_WIDTH: return 1;
			case GL_TEXTURE_WRAP_S: return 1;
			case GL_TEXTURE_WRAP_T: return 1;
#ifdef CR_NV_register_combiners
			case GL_COMBINER_AB_DOT_PRODUCT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_AB_OUTPUT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_BIAS_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_CD_DOT_PRODUCT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_CD_OUTPUT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_COMPONENT_USAGE_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_INPUT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_MAPPING_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_MUX_SUM_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_SCALE_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_NV_register_combiners
			case GL_COMBINER_SUM_OUTPUT_NV: return 1;
#endif /* CR_NV_register_combiners */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_BASE_LEVEL: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */
#ifdef CR_EXT_texture_filter_anisotropic
			case GL_TEXTURE_MAX_ANISOTROPY_EXT: return 1;
#endif /* CR_EXT_texture_filter_anisotropic */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_MAX_LEVEL: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_MAX_LOD: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_MIN_LOD: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_PRIORITY: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */
#ifdef CR_OPENGL_VERSION_1_2
			case GL_TEXTURE_WRAP_R: return 1;
#endif /* CR_OPENGL_VERSION_1_2 */

		default:
			crError( "Unknown parameter name in __lookupComponents: %d", pname );
			break;
	}
	/* NOTREACHED */
	return 0;
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetClipPlane( GLenum plane, GLdouble *equation )
{
	GLdouble  local_equation[4];
	(void) equation;
	cr_server.head_spu->dispatch_table.GetClipPlane( plane, local_equation );
	crServerReturnValue( &(local_equation[0]), 4*sizeof(GLdouble ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetCombinerInputParameterfvNV( GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetCombinerInputParameterfvNV( stage, portion, variable, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetCombinerInputParameterivNV( GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetCombinerInputParameterivNV( stage, portion, variable, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetCombinerOutputParameterfvNV( GLenum stage, GLenum portion, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetCombinerOutputParameterfvNV( stage, portion, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetCombinerOutputParameterivNV( GLenum stage, GLenum portion, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetCombinerOutputParameterivNV( stage, portion, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetCombinerStageParameterfvNV( GLenum stage, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetCombinerStageParameterfvNV( stage, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetFenceivNV( GLuint fence, GLenum pname, GLint *params )
{
	GLint  local_params[1];
	(void) params;
	cr_server.head_spu->dispatch_table.GetFenceivNV( fence, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetFinalCombinerInputParameterfvNV( GLenum variable, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetFinalCombinerInputParameterfvNV( variable, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetFinalCombinerInputParameterivNV( GLenum variable, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetFinalCombinerInputParameterivNV( variable, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetLightfv( GLenum light, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetLightfv( light, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetLightiv( GLenum light, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetLightiv( light, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetMaterialfv( GLenum face, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetMaterialfv( face, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetMaterialiv( GLenum face, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetMaterialiv( face, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetPolygonStipple( GLubyte *mask )
{
	GLubyte  local_mask[128];
	(void) mask;
	cr_server.head_spu->dispatch_table.GetPolygonStipple( local_mask );
	crServerReturnValue( &(local_mask[0]), 128*sizeof(GLubyte ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetProgramParameterdvNV( GLenum target, GLuint index, GLenum pname, GLdouble *params )
{
	GLdouble  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetProgramParameterdvNV( target, index, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLdouble ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetProgramParameterfvNV( GLenum target, GLuint index, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetProgramParameterfvNV( target, index, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetProgramivNV( GLuint id, GLenum pname, GLint *params )
{
	GLint  local_params[1];
	(void) params;
	cr_server.head_spu->dispatch_table.GetProgramivNV( id, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexEnvfv( target, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexEnviv( GLenum target, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexEnviv( target, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexGendv( GLenum coord, GLenum pname, GLdouble *params )
{
	GLdouble  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexGendv( coord, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLdouble ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexGenfv( coord, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexGeniv( GLenum coord, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexGeniv( coord, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexLevelParameterfv( GLenum target, GLint level, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[1];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexLevelParameterfv( target, level, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexLevelParameteriv( GLenum target, GLint level, GLenum pname, GLint *params )
{
	GLint  local_params[1];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexLevelParameteriv( target, level, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexParameterfv( GLenum target, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexParameterfv( target, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTexParameteriv( GLenum target, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTexParameteriv( target, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetTrackMatrixivNV( GLenum target, GLuint address, GLenum pname, GLint *params )
{
	GLint  local_params[1];
	(void) params;
	cr_server.head_spu->dispatch_table.GetTrackMatrixivNV( target, address, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetVertexAttribdvNV( GLuint index, GLenum pname, GLdouble *params )
{
	GLdouble  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetVertexAttribdvNV( index, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLdouble ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetVertexAttribfvNV( GLuint index, GLenum pname, GLfloat *params )
{
	GLfloat  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetVertexAttribfvNV( index, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLfloat ) );
}

void SERVER_DISPATCH_APIENTRY crServerDispatchGetVertexAttribivNV( GLuint index, GLenum pname, GLint *params )
{
	GLint  local_params[4];
	(void) params;
	cr_server.head_spu->dispatch_table.GetVertexAttribivNV( index, pname, local_params );
	crServerReturnValue( &(local_params[0]), __lookupComponents(pname)*sizeof(GLint ) );
}

