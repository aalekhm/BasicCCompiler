#ifndef BASE_H
#define BASE_H

//#define USE_VAO

#ifndef USE_VAO
	//#define USE_VERTEX_POINTERS
#endif

//C/C++
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <map>

#include <gl\glew.h>

// Common
#ifndef NULL
#define NULL     0
#endif

/** Global variable to hold GL errors
 * @script{ignore} */
static GLenum __gl_error_code;

// Assert macros.
#ifdef NDEBUG
#define GP_ASSERT(expression)
#else
#define GP_ASSERT(expression) assert(expression)
#endif


#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) do	{ gl_code; __gl_error_code = glGetError(); GP_ASSERT(__gl_error_code == GL_NO_ERROR); } while(0)
#endif

#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do { /*LOG HERE...*/ assert(0); std::exit(-1); } while(0)
#endif

// Object deletion macro
#define SAFE_DELETE(x) if(x != NULL) { delete x; x = NULL; }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) if(x != NULL) { delete[] x; x = NULL; }

// Ref cleanup macro
#define SAFE_RELEASE(x)	if(x != NULL) { (x)->release(); x = NULL; }

#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)

#endif