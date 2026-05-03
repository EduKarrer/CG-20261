#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
  #error OpenGL header already included; remove it before including glad.h
#endif
#define __gl_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#include <stddef.h>
#include <KHR/khrplatform.h>

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (APIENTRY *GLVULKANPROCNV)(void);

/* GL 1.0 */
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_NONE                           0
#define GL_NO_ERROR                       0
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_TRIANGLES                      0x0004
#define GL_QUADS                          0x0007

/* Enable caps */
#define GL_DEPTH_TEST                     0x0B71
#define GL_BLEND                          0x0BE2
#define GL_CULL_FACE                      0x0B44
#define GL_LINE_SMOOTH                    0x0B20
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037

/* Clear bits */
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400

/* Errors */
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505

/* Polygon modes */
#define GL_POINT                          0x1B00
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408

/* Data types */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_DOUBLE                         0x140A

/* Texture */
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE0                       0x84C0
#define GL_RGBA                           0x1908
#define GL_RGB                            0x1907
#define GL_LINEAR                         0x2601
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F

/* Buffer targets */
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

/* Buffer usage */
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

/* Shader types */
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31

/* Shader/program params */
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84

/* Framebuffer */
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_COMPONENT                0x1902
#define GL_DEPTH_COMPONENT24              0x81A6

/* Blend */
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303

/* Face culling */
#define GL_CCW                            0x0901
#define GL_CW                             0x0900

/* Misc */
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A

/* Function pointer types */
typedef void   (APIENTRYP PFNGLCLEARPROC)(GLbitfield);
typedef void   (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat,GLfloat,GLfloat,GLfloat);
typedef void   (APIENTRYP PFNGLENABLEPROC)(GLenum);
typedef void   (APIENTRYP PFNGLDISABLEPROC)(GLenum);
typedef void   (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum);
typedef void   (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum,GLenum);
typedef void   (APIENTRYP PFNGLVIEWPORTPROC)(GLint,GLint,GLsizei,GLsizei);
typedef void   (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum,GLsizei,GLenum,const void*);
typedef void   (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum,GLint,GLsizei);
typedef void   (APIENTRYP PFNGLPOLYGONMODEPROC)(GLenum,GLenum);
typedef void   (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat,GLfloat);
typedef void   (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat);
typedef void   (APIENTRYP PFNGLPOINTSIZEPROC)(GLfloat);
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
typedef void   (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei,GLuint*);
typedef void   (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum,GLuint);
typedef void   (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const void*);
typedef void   (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum,GLenum,GLint);
typedef void   (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum);
typedef void   (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum);
typedef void   (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei,const GLuint*);
/* VAO */
typedef void   (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void   (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei,const GLuint*);
/* VBO */
typedef void   (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum,GLuint);
typedef void   (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum,GLsizeiptr,const void*,GLenum);
typedef void   (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum,GLintptr,GLsizeiptr,const void*);
typedef void   (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei,const GLuint*);
/* Vertex attribs */
typedef void   (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint,GLint,GLenum,GLboolean,GLsizei,const void*);
typedef void   (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void   (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint);
/* Shaders */
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum);
typedef void   (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint,GLsizei,const GLchar*const*,const GLint*);
typedef void   (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint);
typedef void   (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint,GLenum,GLint*);
typedef void   (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint,GLsizei,GLsizei*,GLchar*);
typedef void   (APIENTRYP PFNGLDELETESHADERPROC)(GLuint);
/* Programs */
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void   (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint,GLuint);
typedef void   (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint);
typedef void   (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint);
typedef void   (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint,GLenum,GLint*);
typedef void   (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint,GLsizei,GLsizei*,GLchar*);
typedef void   (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint);
/* Uniforms */
typedef GLint  (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint,const GLchar*);
typedef void   (APIENTRYP PFNGLUNIFORM1IPROC)(GLint,GLint);
typedef void   (APIENTRYP PFNGLUNIFORM1FPROC)(GLint,GLfloat);
typedef void   (APIENTRYP PFNGLUNIFORM2FPROC)(GLint,GLfloat,GLfloat);
typedef void   (APIENTRYP PFNGLUNIFORM3FPROC)(GLint,GLfloat,GLfloat,GLfloat);
typedef void   (APIENTRYP PFNGLUNIFORM4FPROC)(GLint,GLfloat,GLfloat,GLfloat,GLfloat);
typedef void   (APIENTRYP PFNGLUNIFORM1FVPROC)(GLint,GLsizei,const GLfloat*);
typedef void   (APIENTRYP PFNGLUNIFORM3FVPROC)(GLint,GLsizei,const GLfloat*);
typedef void   (APIENTRYP PFNGLUNIFORM4FVPROC)(GLint,GLsizei,const GLfloat*);
typedef void   (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint,GLsizei,GLboolean,const GLfloat*);
/* Framebuffer */
typedef void   (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum,GLuint);
typedef void   (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum,GLenum,GLenum,GLuint,GLint);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum);
typedef void   (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei,const GLuint*);
typedef void   (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum,GLuint);
typedef void   (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum,GLenum,GLsizei,GLsizei);
typedef void   (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum,GLenum,GLenum,GLuint);
typedef void   (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(GLsizei,const GLuint*);

/* Extern declarations */
extern PFNGLCLEARPROC                    glad_glClear;
extern PFNGLCLEARCOLORPROC               glad_glClearColor;
extern PFNGLENABLEPROC                   glad_glEnable;
extern PFNGLDISABLEPROC                  glad_glDisable;
extern PFNGLDEPTHFUNCPROC                glad_glDepthFunc;
extern PFNGLBLENDFUNCPROC                glad_glBlendFunc;
extern PFNGLVIEWPORTPROC                 glad_glViewport;
extern PFNGLDRAWELEMENTSPROC             glad_glDrawElements;
extern PFNGLDRAWARRAYSPROC               glad_glDrawArrays;
extern PFNGLPOLYGONMODEPROC              glad_glPolygonMode;
extern PFNGLPOLYGONOFFSETPROC            glad_glPolygonOffset;
extern PFNGLLINEWIDTHPROC                glad_glLineWidth;
extern PFNGLPOINTSIZEPROC                glad_glPointSize;
extern PFNGLGETERRORPROC                 glad_glGetError;
extern PFNGLGENTEXTURESPROC              glad_glGenTextures;
extern PFNGLBINDTEXTUREPROC              glad_glBindTexture;
extern PFNGLTEXIMAGE2DPROC               glad_glTexImage2D;
extern PFNGLTEXPARAMETERIPROC            glad_glTexParameteri;
extern PFNGLGENERATEMIPMAPPROC           glad_glGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC            glad_glActiveTexture;
extern PFNGLDELETETEXTURESPROC           glad_glDeleteTextures;
extern PFNGLGENVERTEXARRAYSPROC          glad_glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC          glad_glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC       glad_glDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC               glad_glGenBuffers;
extern PFNGLBINDBUFFERPROC               glad_glBindBuffer;
extern PFNGLBUFFERDATAPROC               glad_glBufferData;
extern PFNGLBUFFERSUBDATAPROC            glad_glBufferSubData;
extern PFNGLDELETEBUFFERSPROC            glad_glDeleteBuffers;
extern PFNGLVERTEXATTRIBPOINTERPROC      glad_glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glad_glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
extern PFNGLCREATESHADERPROC             glad_glCreateShader;
extern PFNGLSHADERSOURCEPROC             glad_glShaderSource;
extern PFNGLCOMPILESHADERPROC            glad_glCompileShader;
extern PFNGLGETSHADERIVPROC              glad_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC         glad_glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC             glad_glDeleteShader;
extern PFNGLCREATEPROGRAMPROC            glad_glCreateProgram;
extern PFNGLATTACHSHADERPROC             glad_glAttachShader;
extern PFNGLLINKPROGRAMPROC              glad_glLinkProgram;
extern PFNGLUSEPROGRAMPROC               glad_glUseProgram;
extern PFNGLGETPROGRAMIVPROC             glad_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glad_glGetProgramInfoLog;
extern PFNGLDELETEPROGRAMPROC            glad_glDeleteProgram;
extern PFNGLGETUNIFORMLOCATIONPROC       glad_glGetUniformLocation;
extern PFNGLUNIFORM1IPROC                glad_glUniform1i;
extern PFNGLUNIFORM1FPROC                glad_glUniform1f;
extern PFNGLUNIFORM2FPROC                glad_glUniform2f;
extern PFNGLUNIFORM3FPROC                glad_glUniform3f;
extern PFNGLUNIFORM4FPROC                glad_glUniform4f;
extern PFNGLUNIFORM1FVPROC               glad_glUniform1fv;
extern PFNGLUNIFORM3FVPROC               glad_glUniform3fv;
extern PFNGLUNIFORM4FVPROC               glad_glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC         glad_glUniformMatrix4fv;
extern PFNGLGENFRAMEBUFFERSPROC          glad_glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC          glad_glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC     glad_glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC   glad_glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC       glad_glDeleteFramebuffers;
extern PFNGLGENRENDERBUFFERSPROC         glad_glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC         glad_glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC      glad_glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC  glad_glFramebufferRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC      glad_glDeleteRenderbuffers;

/* Macros */
#define glClear                    glad_glClear
#define glClearColor               glad_glClearColor
#define glEnable                   glad_glEnable
#define glDisable                  glad_glDisable
#define glDepthFunc                glad_glDepthFunc
#define glBlendFunc                glad_glBlendFunc
#define glViewport                 glad_glViewport
#define glDrawElements             glad_glDrawElements
#define glDrawArrays               glad_glDrawArrays
#define glPolygonMode              glad_glPolygonMode
#define glPolygonOffset            glad_glPolygonOffset
#define glLineWidth                glad_glLineWidth
#define glPointSize                glad_glPointSize
#define glGetError                 glad_glGetError
#define glGenTextures              glad_glGenTextures
#define glBindTexture              glad_glBindTexture
#define glTexImage2D               glad_glTexImage2D
#define glTexParameteri            glad_glTexParameteri
#define glGenerateMipmap           glad_glGenerateMipmap
#define glActiveTexture            glad_glActiveTexture
#define glDeleteTextures           glad_glDeleteTextures
#define glGenVertexArrays          glad_glGenVertexArrays
#define glBindVertexArray          glad_glBindVertexArray
#define glDeleteVertexArrays       glad_glDeleteVertexArrays
#define glGenBuffers               glad_glGenBuffers
#define glBindBuffer               glad_glBindBuffer
#define glBufferData               glad_glBufferData
#define glBufferSubData            glad_glBufferSubData
#define glDeleteBuffers            glad_glDeleteBuffers
#define glVertexAttribPointer      glad_glVertexAttribPointer
#define glEnableVertexAttribArray  glad_glEnableVertexAttribArray
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
#define glCreateShader             glad_glCreateShader
#define glShaderSource             glad_glShaderSource
#define glCompileShader            glad_glCompileShader
#define glGetShaderiv              glad_glGetShaderiv
#define glGetShaderInfoLog         glad_glGetShaderInfoLog
#define glDeleteShader             glad_glDeleteShader
#define glCreateProgram            glad_glCreateProgram
#define glAttachShader             glad_glAttachShader
#define glLinkProgram              glad_glLinkProgram
#define glUseProgram               glad_glUseProgram
#define glGetProgramiv             glad_glGetProgramiv
#define glGetProgramInfoLog        glad_glGetProgramInfoLog
#define glDeleteProgram            glad_glDeleteProgram
#define glGetUniformLocation       glad_glGetUniformLocation
#define glUniform1i                glad_glUniform1i
#define glUniform1f                glad_glUniform1f
#define glUniform2f                glad_glUniform2f
#define glUniform3f                glad_glUniform3f
#define glUniform4f                glad_glUniform4f
#define glUniform1fv               glad_glUniform1fv
#define glUniform3fv               glad_glUniform3fv
#define glUniform4fv               glad_glUniform4fv
#define glUniformMatrix4fv         glad_glUniformMatrix4fv
#define glGenFramebuffers          glad_glGenFramebuffers
#define glBindFramebuffer          glad_glBindFramebuffer
#define glFramebufferTexture2D     glad_glFramebufferTexture2D
#define glCheckFramebufferStatus   glad_glCheckFramebufferStatus
#define glDeleteFramebuffers       glad_glDeleteFramebuffers
#define glGenRenderbuffers         glad_glGenRenderbuffers
#define glBindRenderbuffer         glad_glBindRenderbuffer
#define glRenderbufferStorage      glad_glRenderbufferStorage
#define glFramebufferRenderbuffer  glad_glFramebufferRenderbuffer
#define glDeleteRenderbuffers      glad_glDeleteRenderbuffers

typedef void* (*GLADloadproc)(const char *name);

#ifdef __cplusplus
extern "C" {
#endif

int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#endif /* __glad_h_ */
