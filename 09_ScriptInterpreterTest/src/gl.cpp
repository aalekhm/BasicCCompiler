#include "gl.h"

//OpenGL1.0

void (GL_FUNCPTR* _ptr_glCullFace)(GLenum mode) = nullptr;
void (GL_FUNCPTR* _ptr_glFrontFace)(GLenum mode) = nullptr;
void (GL_FUNCPTR* _ptr_glHint)(GLenum target, GLenum mode) = nullptr;
void (GL_FUNCPTR* _ptr_glLineWidth)(GLfloat width) = nullptr;
void (GL_FUNCPTR* _ptr_glPointSize)(GLfloat size) = nullptr;
void (GL_FUNCPTR* _ptr_glPolygonMode)(GLenum face, GLenum mode) = nullptr;
void (GL_FUNCPTR* _ptr_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;
void (GL_FUNCPTR* _ptr_glTexParameterf)(GLenum target, GLenum pname, GLfloat param) = nullptr;
void (GL_FUNCPTR* _ptr_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params) = nullptr;
void (GL_FUNCPTR* _ptr_glTexParameteri)(GLenum target, GLenum pname, GLint param) = nullptr;
void (GL_FUNCPTR* _ptr_glTexParameteriv)(GLenum target, GLenum pname, const GLint *params) = nullptr;
void (GL_FUNCPTR* _ptr_glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR* _ptr_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR* _ptr_glDrawBuffer)(GLenum buf) = nullptr;
void (GL_FUNCPTR* _ptr_glClear)(GLbitfield mask);
void (GL_FUNCPTR* _ptr_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = nullptr;
void (GL_FUNCPTR* _ptr_glClearStencil)(GLint s) = nullptr;
void (GL_FUNCPTR* _ptr_glClearDepth)(GLclampd depth) = nullptr;
void (GL_FUNCPTR* _ptr_glStencilMask)(GLuint mask) = nullptr;
void (GL_FUNCPTR* _ptr_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) = nullptr;
void (GL_FUNCPTR* _ptr_glDepthMask)(GLboolean flag) = nullptr;
void (GL_FUNCPTR* _ptr_glDisable)(GLenum cap) = nullptr;
void (GL_FUNCPTR* _ptr_glEnable)(GLenum cap) = nullptr;
void (GL_FUNCPTR* _ptr_glFinish)(void) = nullptr;
void (GL_FUNCPTR* _ptr_glFlush)(void) = nullptr;
void (GL_FUNCPTR* _ptr_glBlendFunc)(GLenum sfactor, GLenum dfactor) = nullptr;
void (GL_FUNCPTR* _ptr_glLogicOp)(GLenum opcode) = nullptr;
void (GL_FUNCPTR* _ptr_glStencilFunc)(GLenum func, GLint ref, GLuint mask) = nullptr;
void (GL_FUNCPTR* _ptr_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass) = nullptr;
void (GL_FUNCPTR* _ptr_glDepthFunc)(GLenum func) = nullptr;
void (GL_FUNCPTR* _ptr_glPixelStoref)(GLenum pname, GLfloat param) = nullptr;
void (GL_FUNCPTR* _ptr_glPixelStorei)(GLenum pname, GLint param) = nullptr;
void (GL_FUNCPTR* _ptr_glReadBuffer)(GLenum src) = nullptr;
void (GL_FUNCPTR* _ptr_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) = nullptr;
void (GL_FUNCPTR* _ptr_glGetBooleanv)(GLenum pname, GLboolean *data) = nullptr;
void (GL_FUNCPTR* _ptr_glGetDoublev)(GLenum pname, GLdouble *data) = nullptr;
GLenum(GL_FUNCPTR* _ptr_glGetError)(void) = nullptr;
void (GL_FUNCPTR* _ptr_glGetFloatv)(GLenum pname, GLfloat *data) = nullptr;
void (GL_FUNCPTR* _ptr_glGetIntegerv)(GLenum pname, GLint *data) = nullptr;
const GLubyte *(GL_FUNCPTR* _ptr_glGetString)(GLenum name) = nullptr;
void (GL_FUNCPTR* _ptr_glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels) = nullptr;
void (GL_FUNCPTR* _ptr_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params) = nullptr;
void (GL_FUNCPTR* _ptr_glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR* _ptr_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params) = nullptr;
void (GL_FUNCPTR* _ptr_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params) = nullptr;
GLboolean(GL_FUNCPTR* _ptr_glIsEnabled)(GLenum cap) = nullptr;
void (GL_FUNCPTR* _ptr_glDepthRange)(GLdouble _near, GLdouble _far) = nullptr;
void (GL_FUNCPTR* _ptr_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;

//OpenGL1.1
//core

void (GL_FUNCPTR*_ptr_glDrawArrays)(GLenum mode, GLint first, GLsizei count) = nullptr;
void (GL_FUNCPTR*_ptr_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices) = nullptr;
void (GL_FUNCPTR*_ptr_glGetPointerv)(GLenum pname, void **params) = nullptr;
void (GL_FUNCPTR*_ptr_glPolygonOffset)(GLfloat factor, GLfloat units) = nullptr;
void (GL_FUNCPTR*_ptr_glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) = nullptr;
void (GL_FUNCPTR*_ptr_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) = nullptr;
void (GL_FUNCPTR*_ptr_glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) = nullptr;
void (GL_FUNCPTR*_ptr_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;
void (GL_FUNCPTR*_ptr_glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR*_ptr_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR*_ptr_glBindTexture)(GLenum target, GLuint texture) = nullptr;
void (GL_FUNCPTR*_ptr_glDeleteTextures)(GLsizei n, const GLuint *textures) = nullptr;
void (GL_FUNCPTR*_ptr_glGenTextures)(GLsizei n, GLuint *textures) = nullptr;
GLboolean(GL_FUNCPTR*_ptr_glIsTexture)(GLuint texture) = nullptr;

//compatibility
void(GL_FUNCPTR * _ptr_glMatrixMode)(GLenum mode) = nullptr;
void(GL_FUNCPTR * _ptr_glLoadIdentity)() = nullptr;
void(GL_FUNCPTR * _ptr_glLoadMatrixf)(const GLfloat*) = nullptr;

void(GL_FUNCPTR * _ptr_glBegin)(GLenum) = nullptr;
void(GL_FUNCPTR * _ptr_glEnd)(void) = nullptr;
void(GL_FUNCPTR * _ptr_glColor3f)(GLfloat, GLfloat, GLfloat) = nullptr;
void(GL_FUNCPTR * _ptr_glVertex2f)(GLfloat, GLfloat) = nullptr;

//OpenGL1.2
//core 

void (GL_FUNCPTR * _ptr_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) = nullptr;
void (GL_FUNCPTR * _ptr_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR * _ptr_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) = nullptr;
void (GL_FUNCPTR * _ptr_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;

//OpenGL1.3
//core

void (GL_FUNCPTR * _ptr_glActiveTexture)(GLenum texture) = nullptr;
void (GL_FUNCPTR * _ptr_glSampleCoverage)(GLfloat value, GLboolean invert) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glGetCompressedTexImage)(GLenum target, GLint level, void *img) = nullptr;

//OpenGL1.4
//core

void (GL_FUNCPTR * _ptr_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) = nullptr;
void (GL_FUNCPTR * _ptr_glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) = nullptr;
void (GL_FUNCPTR * _ptr_glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount) = nullptr;
void (GL_FUNCPTR * _ptr_glPointParameterf)(GLenum pname, GLfloat param) = nullptr;
void (GL_FUNCPTR * _ptr_glPointParameterfv)(GLenum pname, const GLfloat *params) = nullptr;
void (GL_FUNCPTR * _ptr_glPointParameteri)(GLenum pname, GLint param) = nullptr;
void (GL_FUNCPTR * _ptr_glPointParameteriv)(GLenum pname, const GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = nullptr;
void (GL_FUNCPTR * _ptr_glBlendEquation)(GLenum mode) = nullptr;

//OpenGL1.5
//core

void (GL_FUNCPTR * _ptr_glGenQueries)(GLsizei n, GLuint *ids) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteQueries)(GLsizei n, const GLuint *ids) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsQuery)(GLuint id) = nullptr;
void (GL_FUNCPTR * _ptr_glBeginQuery)(GLenum target, GLuint id) = nullptr;
void (GL_FUNCPTR * _ptr_glEndQuery)(GLenum target) = nullptr;
void (GL_FUNCPTR * _ptr_glGetQueryiv)(GLenum target, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glBindBuffer)(GLenum target, GLuint buffer) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteBuffers)(GLsizei n, const GLuint *buffers) = nullptr;
void (GL_FUNCPTR * _ptr_glGenBuffers)(GLsizei n, GLuint *buffers) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsBuffer)(GLuint buffer) = nullptr;
void (GL_FUNCPTR * _ptr_glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage) = nullptr;
void (GL_FUNCPTR * _ptr_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) = nullptr;
void (GL_FUNCPTR * _ptr_glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, void *data) = nullptr;
void *(GL_FUNCPTR * _ptr_glMapBuffer)(GLenum target, GLenum access) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glUnmapBuffer)(GLenum target) = nullptr;
void (GL_FUNCPTR * _ptr_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetBufferPointerv)(GLenum target, GLenum pname, void **params) = nullptr;

//OpenGL2.0
//core
void (GL_FUNCPTR * _ptr_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha) = nullptr;
void (GL_FUNCPTR * _ptr_glDrawBuffers)(GLsizei n, const GLenum *bufs) = nullptr;
void (GL_FUNCPTR * _ptr_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) = nullptr;
void (GL_FUNCPTR * _ptr_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask) = nullptr;
void (GL_FUNCPTR * _ptr_glStencilMaskSeparate)(GLenum face, GLuint mask) = nullptr;
void (GL_FUNCPTR * _ptr_glAttachShader)(GLuint program, GLuint shader) = nullptr;
void (GL_FUNCPTR * _ptr_glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glCompileShader)(GLuint shader);
GLuint(GL_FUNCPTR * _ptr_glCreateProgram)(void) = nullptr;
GLuint(GL_FUNCPTR * _ptr_glCreateShader)(GLenum type) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteProgram)(GLuint program) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteShader)(GLuint shader) = nullptr;
void (GL_FUNCPTR * _ptr_glDetachShader)(GLuint program, GLuint shader) = nullptr;
void (GL_FUNCPTR * _ptr_glDisableVertexAttribArray)(GLuint index) = nullptr;
void (GL_FUNCPTR * _ptr_glEnableVertexAttribArray)(GLuint index) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) = nullptr;
GLint(GL_FUNCPTR * _ptr_glGetAttribLocation)(GLuint program, const GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glGetProgramiv)(GLuint program, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = nullptr;
void (GL_FUNCPTR * _ptr_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = nullptr;
void (GL_FUNCPTR * _ptr_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) = nullptr;
GLint(GL_FUNCPTR * _ptr_glGetUniformLocation)(GLuint program, const GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glGetUniformfv)(GLuint program, GLint location, GLfloat *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetUniformiv)(GLuint program, GLint location, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsProgram)(GLuint program) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsShader)(GLuint shader) = nullptr;
void (GL_FUNCPTR * _ptr_glLinkProgram)(GLuint program) = nullptr;
void (GL_FUNCPTR * _ptr_glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) = nullptr;
void (GL_FUNCPTR * _ptr_glUseProgram)(GLuint program) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1f)(GLint location, GLfloat v0) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2f)(GLint location, GLfloat v0, GLfloat v1) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1i)(GLint location, GLint v0) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2i)(GLint location, GLint v0, GLint v1) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1fv)(GLint location, GLsizei count, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2fv)(GLint location, GLsizei count, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1iv)(GLint location, GLsizei count, const GLint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2iv)(GLint location, GLsizei count, const GLint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3iv)(GLint location, GLsizei count, const GLint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4iv)(GLint location, GLsizei count, const GLint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glValidateProgram)(GLuint program) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1d)(GLuint index, GLdouble x) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1dv)(GLuint index, const GLdouble *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1f)(GLuint index, GLfloat x) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1fv)(GLuint index, const GLfloat *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1s)(GLuint index, GLshort x) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib1sv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2dv)(GLuint index, const GLdouble *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2fv)(GLuint index, const GLfloat *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2s)(GLuint index, GLshort x, GLshort y) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib2sv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3dv)(GLuint index, const GLdouble *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3fv)(GLuint index, const GLfloat *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib3sv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nbv)(GLuint index, const GLbyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Niv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nsv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nubv)(GLuint index, const GLubyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nuiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4Nusv)(GLuint index, const GLushort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4bv)(GLuint index, const GLbyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4dv)(GLuint index, const GLdouble *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4fv)(GLuint index, const GLfloat *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4iv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4sv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4ubv)(GLuint index, const GLubyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4uiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttrib4usv)(GLuint index, const GLushort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) = nullptr;

//OpenGL2.1

void (GL_FUNCPTR*_ptr_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR*_ptr_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR*_ptr_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR*_ptr_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR*_ptr_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;
void (GL_FUNCPTR*_ptr_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;

//OpenGL3.0
void (GL_FUNCPTR * _ptr_glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) = nullptr;
void (GL_FUNCPTR * _ptr_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data) = nullptr;
void (GL_FUNCPTR * _ptr_glGetIntegeri_v)(GLenum target, GLuint index, GLint *data) = nullptr;
void (GL_FUNCPTR * _ptr_glEnablei)(GLenum target, GLuint index) = nullptr;
void (GL_FUNCPTR * _ptr_glDisablei)(GLenum target, GLuint index) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsEnabledi)(GLenum target, GLuint index) = nullptr;
void (GL_FUNCPTR * _ptr_glBeginTransformFeedback)(GLenum primitiveMode) = nullptr;
void (GL_FUNCPTR * _ptr_glEndTransformFeedback)(void) = nullptr;
void (GL_FUNCPTR * _ptr_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) = nullptr;
void (GL_FUNCPTR * _ptr_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer) = nullptr;
void (GL_FUNCPTR * _ptr_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) = nullptr;
void (GL_FUNCPTR * _ptr_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glClampColor)(GLenum target, GLenum clamp) = nullptr;
void (GL_FUNCPTR * _ptr_glBeginConditionalRender)(GLuint id, GLenum mode) = nullptr;
void (GL_FUNCPTR * _ptr_glEndConditionalRender)(void) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI1i)(GLuint index, GLint x) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI2i)(GLuint index, GLint x, GLint y) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI1ui)(GLuint index, GLuint x) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI1iv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI2iv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI3iv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4iv)(GLuint index, const GLint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI1uiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI2uiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI3uiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4uiv)(GLuint index, const GLuint *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4bv)(GLuint index, const GLbyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4sv)(GLuint index, const GLshort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4ubv)(GLuint index, const GLubyte *v) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribI4usv)(GLuint index, const GLushort *v) = nullptr;
void (GL_FUNCPTR * _ptr_glGetUniformuiv)(GLuint program, GLint location, GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name) = nullptr;
GLint(GL_FUNCPTR * _ptr_glGetFragDataLocation)(GLuint program, const GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1ui)(GLint location, GLuint v0) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2ui)(GLint location, GLuint v0, GLuint v1) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value) = nullptr;
void (GL_FUNCPTR * _ptr_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) = nullptr;
const GLubyte *(GL_FUNCPTR * _ptr_glGetStringi)(GLenum name, GLuint index) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsRenderbuffer)(GLuint renderbuffer) = nullptr;
void (GL_FUNCPTR * _ptr_glBindRenderbuffer)(GLenum target, GLuint renderbuffer) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers) = nullptr;
void (GL_FUNCPTR * _ptr_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers) = nullptr;
void (GL_FUNCPTR * _ptr_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = nullptr;
void (GL_FUNCPTR * _ptr_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsFramebuffer)(GLuint framebuffer) = nullptr;
void (GL_FUNCPTR * _ptr_glBindFramebuffer)(GLenum target, GLuint framebuffer) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers) = nullptr;
void (GL_FUNCPTR * _ptr_glGenFramebuffers)(GLsizei n, GLuint *framebuffers) = nullptr;
GLenum(GL_FUNCPTR * _ptr_glCheckFramebufferStatus)(GLenum target) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = nullptr;
void (GL_FUNCPTR * _ptr_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGenerateMipmap)(GLenum target) = nullptr;
void (GL_FUNCPTR * _ptr_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) = nullptr;
void (GL_FUNCPTR * _ptr_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) = nullptr;
void *(GL_FUNCPTR * _ptr_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) = nullptr;
void (GL_FUNCPTR * _ptr_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length) = nullptr;
void (GL_FUNCPTR * _ptr_glBindVertexArray)(GLuint _array) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays) = nullptr;
void (GL_FUNCPTR * _ptr_glGenVertexArrays)(GLsizei n, GLuint *arrays) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsVertexArray)(GLuint _array) = nullptr;


//OpenGL3.1
//core

void (GL_FUNCPTR * _ptr_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) = nullptr;
void (GL_FUNCPTR * _ptr_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) = nullptr;
void (GL_FUNCPTR * _ptr_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer) = nullptr;
void (GL_FUNCPTR * _ptr_glPrimitiveRestartIndex)(GLuint index) = nullptr;
void (GL_FUNCPTR * _ptr_glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) = nullptr;
void (GL_FUNCPTR * _ptr_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) = nullptr;
GLuint(GL_FUNCPTR * _ptr_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) = nullptr;
void (GL_FUNCPTR * _ptr_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) = nullptr;

//OpenGL3.2
//core

void (GL_FUNCPTR * _ptr_glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) = nullptr;
void (GL_FUNCPTR * _ptr_glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) = nullptr;
void (GL_FUNCPTR * _ptr_glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) = nullptr;
void (GL_FUNCPTR * _ptr_glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex) = nullptr;
void (GL_FUNCPTR * _ptr_glProvokingVertex)(GLenum mode) = nullptr;
GLsync(GL_FUNCPTR * _ptr_glFenceSync)(GLenum condition, GLbitfield flags) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsSync)(GLsync sync) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteSync)(GLsync sync) = nullptr;
GLenum(GL_FUNCPTR * _ptr_glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout) = nullptr;
void (GL_FUNCPTR * _ptr_glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout) = nullptr;
void (GL_FUNCPTR * _ptr_glGetInteger64v)(GLenum pname, GLint64 *data) = nullptr;
void (GL_FUNCPTR * _ptr_glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) = nullptr;
void (GL_FUNCPTR * _ptr_glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data) = nullptr;
void (GL_FUNCPTR * _ptr_glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params) = nullptr;
void (GL_FUNCPTR * _ptr_glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level) = nullptr;
void (GL_FUNCPTR * _ptr_glTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) = nullptr;
void (GL_FUNCPTR * _ptr_glTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) = nullptr;
void (GL_FUNCPTR * _ptr_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val) = nullptr;
void (GL_FUNCPTR * _ptr_glSampleMaski)(GLuint maskNumber, GLbitfield mask) = nullptr;

//OpenGL3.3
//core

void (GL_FUNCPTR * _ptr_glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) = nullptr;
GLint(GL_FUNCPTR * _ptr_glGetFragDataIndex)(GLuint program, const GLchar *name) = nullptr;
void (GL_FUNCPTR * _ptr_glGenSamplers)(GLsizei count, GLuint *samplers) = nullptr;
void (GL_FUNCPTR * _ptr_glDeleteSamplers)(GLsizei count, const GLuint *samplers) = nullptr;
GLboolean(GL_FUNCPTR * _ptr_glIsSampler)(GLuint sampler) = nullptr;
void (GL_FUNCPTR * _ptr_glBindSampler)(GLuint unit, GLuint sampler) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param) = nullptr;
void (GL_FUNCPTR * _ptr_glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param) = nullptr;
void (GL_FUNCPTR * _ptr_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params) = nullptr;
void (GL_FUNCPTR * _ptr_glQueryCounter)(GLuint id, GLenum target) = nullptr;
void (GL_FUNCPTR * _ptr_glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64 *params) = nullptr;
void (GL_FUNCPTR * _ptr_glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribDivisor)(GLuint index, GLuint divisor) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value) = nullptr;
void (GL_FUNCPTR * _ptr_glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) = nullptr;


void * GetGLProcAddress(const char *name){
	static HMODULE glMod = NULL;
	PROC pFunc = wglGetProcAddress((LPCSTR)name);

	if (pFunc) return (void *)pFunc;

	if (NULL == glMod)
		glMod = GetModuleHandleA("OpenGL32.dll");

	return (void *)GetProcAddress(glMod, (LPCSTR)name);
}
