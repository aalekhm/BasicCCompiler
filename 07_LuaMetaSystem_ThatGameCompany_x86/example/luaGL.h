#include "extern/gl.h"
#include "meta/MetaFunction.h"
#include "meta/MetaVariable.h"

#define META_REGISTER_GL_CONSTANT(constant)\
    GLuint g_##constant = constant;\
    MetaVariable g_Meta##constant(#constant, g_##constant);

META_REGISTER_GL_CONSTANT(GL_COLOR_BUFFER_BIT);
META_REGISTER_GL_CONSTANT(GL_DEPTH_BUFFER_BIT);
META_REGISTER_GL_CONSTANT(GL_TRIANGLE_STRIP);
META_REGISTER_GL_CONSTANT(GL_QUADS);
META_REGISTER_GL_CONSTANT(GL_MODELVIEW);
META_REGISTER_GL_CONSTANT(GL_PROJECTION);

META_REGISTER_FUN(glMatrixMode, "glMatrixMode");
META_REGISTER_FUN(glLoadIdentity, "glLoadIdentity");
META_REGISTER_FUN(glBegin, "glBegin");
META_REGISTER_FUN(glEnd, "glEnd");
META_REGISTER_FUN(glClearColor, "glClearColor");
META_REGISTER_FUN(glClear, "glClear");
META_REGISTER_FUN(glColor3f, "glColor3f");
META_REGISTER_FUN(glVertex2f, "glVertex2f");



//not really a gl function but useful for demonstration
void createGLRotationMatrix(float32 _x, float32 _y, float32 _angle){
    float32 m[] = {
        std::cosf(_angle), -std::sinf(_angle), 0, 0,
        std::sinf(_angle), std::cosf(_angle) , 0, 0,
        0                , 0                 , 1, 0,
        _x               , _y                , 0, 1        
    };
    glLoadMatrixf(m);
}

META_REGISTER_FUN(createGLRotationMatrix, "glRotationzf");
