#ifndef __GTULU_TYPES_H__
#define __GTULU_TYPES_H__

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
typedef std::ptrdiff_t   GLintptr;
typedef std::ptrdiff_t   GLsizeiptr;
typedef std::int64_t     GLint64;
typedef std::uint64_t    GLuint64;
typedef struct __GLsync* GLsync;
typedef void             (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void(*EGLProc)(void);
typedef int EGLint;
typedef unsigned int EGLBoolean;
typedef unsigned int EGLenum;
typedef void *EGLConfig;
typedef void *EGLContext;
typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLClientBuffer;

typedef void *EGLNativePixmapType;
typedef void *EGLNativeDisplayType;
typedef void *EGLNativeWindowType;

#endif // ifndef __GTULU_TYPES_H__
