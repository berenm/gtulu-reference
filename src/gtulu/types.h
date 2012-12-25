#ifndef __GTULU_TYPES_H__
#define __GTULU_TYPES_H__

#include <boost/config.hpp>
#include <cstdint>

typedef unsigned int     GLenum;
typedef unsigned char    GLboolean;
typedef unsigned int     GLbitfield;
typedef signed char      GLbyte;
typedef short            GLshort;
typedef int              GLint;
typedef int              GLsizei;
typedef unsigned char    GLubyte;
typedef unsigned short   GLushort;
typedef unsigned int     GLuint;
typedef unsigned short   GLhalf;
typedef float            GLfloat;
typedef float            GLclampf;
typedef double           GLdouble;
typedef double           GLclampd;
typedef void             GLvoid;
typedef char             GLchar;
typedef ptrdiff_t        GLintptr;
typedef ptrdiff_t        GLsizeiptr;
typedef int64_t          GLint64;
typedef uint64_t         GLuint64;
typedef struct __GLsync* GLsync;
typedef void             (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

#ifdef __IN_GTULU_OPENGL_API__
# define GTULU_API_EXPORT __attribute__((visibility("default")))
# define GTULU_API_IMPORT BOOST_SYMBOL_IMPORT
#else /* ifdef __IN_GTULU_OPENGL_API__ */
# define GTULU_API_EXPORT BOOST_SYMBOL_IMPORT
# define GTULU_API_IMPORT __attribute__((visibility("default")))
#endif /* ifdef __IN_GTULU_OPENGL_API__ */

#endif // ifndef __GTULU_TYPES_H__
