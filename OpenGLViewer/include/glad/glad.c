#include "glad/glad.h"
#include <string.h>
#include <stdio.h>

/* Definitions */
PFNGLCLEARPROC                    glad_glClear                    = NULL;
PFNGLCLEARCOLORPROC               glad_glClearColor               = NULL;
PFNGLENABLEPROC                   glad_glEnable                   = NULL;
PFNGLDISABLEPROC                  glad_glDisable                  = NULL;
PFNGLDEPTHFUNCPROC                glad_glDepthFunc                = NULL;
PFNGLBLENDFUNCPROC                glad_glBlendFunc                = NULL;
PFNGLVIEWPORTPROC                 glad_glViewport                 = NULL;
PFNGLDRAWELEMENTSPROC             glad_glDrawElements             = NULL;
PFNGLDRAWARRAYSPROC               glad_glDrawArrays               = NULL;
PFNGLPOLYGONMODEPROC              glad_glPolygonMode              = NULL;
PFNGLPOLYGONOFFSETPROC            glad_glPolygonOffset            = NULL;
PFNGLLINEWIDTHPROC                glad_glLineWidth                = NULL;
PFNGLPOINTSIZEPROC                glad_glPointSize                = NULL;
PFNGLGETERRORPROC                 glad_glGetError                 = NULL;
PFNGLGENTEXTURESPROC              glad_glGenTextures              = NULL;
PFNGLBINDTEXTUREPROC              glad_glBindTexture              = NULL;
PFNGLTEXIMAGE2DPROC               glad_glTexImage2D               = NULL;
PFNGLTEXPARAMETERIPROC            glad_glTexParameteri            = NULL;
PFNGLGENERATEMIPMAPPROC           glad_glGenerateMipmap           = NULL;
PFNGLACTIVETEXTUREPROC            glad_glActiveTexture            = NULL;
PFNGLDELETETEXTURESPROC           glad_glDeleteTextures           = NULL;
PFNGLGENVERTEXARRAYSPROC          glad_glGenVertexArrays          = NULL;
PFNGLBINDVERTEXARRAYPROC          glad_glBindVertexArray          = NULL;
PFNGLDELETEVERTEXARRAYSPROC       glad_glDeleteVertexArrays       = NULL;
PFNGLGENBUFFERSPROC               glad_glGenBuffers               = NULL;
PFNGLBINDBUFFERPROC               glad_glBindBuffer               = NULL;
PFNGLBUFFERDATAPROC               glad_glBufferData               = NULL;
PFNGLBUFFERSUBDATAPROC            glad_glBufferSubData            = NULL;
PFNGLDELETEBUFFERSPROC            glad_glDeleteBuffers            = NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glad_glVertexAttribPointer      = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glad_glEnableVertexAttribArray  = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLCREATESHADERPROC             glad_glCreateShader             = NULL;
PFNGLSHADERSOURCEPROC             glad_glShaderSource             = NULL;
PFNGLCOMPILESHADERPROC            glad_glCompileShader            = NULL;
PFNGLGETSHADERIVPROC              glad_glGetShaderiv              = NULL;
PFNGLGETSHADERINFOLOGPROC         glad_glGetShaderInfoLog         = NULL;
PFNGLDELETESHADERPROC             glad_glDeleteShader             = NULL;
PFNGLCREATEPROGRAMPROC            glad_glCreateProgram            = NULL;
PFNGLATTACHSHADERPROC             glad_glAttachShader             = NULL;
PFNGLLINKPROGRAMPROC              glad_glLinkProgram              = NULL;
PFNGLUSEPROGRAMPROC               glad_glUseProgram               = NULL;
PFNGLGETPROGRAMIVPROC             glad_glGetProgramiv             = NULL;
PFNGLGETPROGRAMINFOLOGPROC        glad_glGetProgramInfoLog        = NULL;
PFNGLDELETEPROGRAMPROC            glad_glDeleteProgram            = NULL;
PFNGLGETUNIFORMLOCATIONPROC       glad_glGetUniformLocation       = NULL;
PFNGLUNIFORM1IPROC                glad_glUniform1i                = NULL;
PFNGLUNIFORM1FPROC                glad_glUniform1f                = NULL;
PFNGLUNIFORM2FPROC                glad_glUniform2f                = NULL;
PFNGLUNIFORM3FPROC                glad_glUniform3f                = NULL;
PFNGLUNIFORM4FPROC                glad_glUniform4f                = NULL;
PFNGLUNIFORM1FVPROC               glad_glUniform1fv               = NULL;
PFNGLUNIFORM3FVPROC               glad_glUniform3fv               = NULL;
PFNGLUNIFORM4FVPROC               glad_glUniform4fv               = NULL;
PFNGLUNIFORMMATRIX4FVPROC         glad_glUniformMatrix4fv         = NULL;
PFNGLGENFRAMEBUFFERSPROC          glad_glGenFramebuffers          = NULL;
PFNGLBINDFRAMEBUFFERPROC          glad_glBindFramebuffer          = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC     glad_glFramebufferTexture2D     = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC   glad_glCheckFramebufferStatus   = NULL;
PFNGLDELETEFRAMEBUFFERSPROC       glad_glDeleteFramebuffers       = NULL;
PFNGLGENRENDERBUFFERSPROC         glad_glGenRenderbuffers         = NULL;
PFNGLBINDRENDERBUFFERPROC         glad_glBindRenderbuffer         = NULL;
PFNGLRENDERBUFFERSTORAGEPROC      glad_glRenderbufferStorage      = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC  glad_glFramebufferRenderbuffer  = NULL;
PFNGLDELETERENDERBUFFERSPROC      glad_glDeleteRenderbuffers      = NULL;

static GLADloadproc glad_loader = NULL;

static void* glad_get_proc(const char* name) {
    if (!glad_loader) return NULL;
    return glad_loader(name);
}

int gladLoadGLLoader(GLADloadproc load) {
    glad_loader = load;
    #define LOAD(fn, TYPE) glad_##fn = (TYPE)glad_get_proc(#fn)
    LOAD(glClear,                    PFNGLCLEARPROC);
    LOAD(glClearColor,               PFNGLCLEARCOLORPROC);
    LOAD(glEnable,                   PFNGLENABLEPROC);
    LOAD(glDisable,                  PFNGLDISABLEPROC);
    LOAD(glDepthFunc,                PFNGLDEPTHFUNCPROC);
    LOAD(glBlendFunc,                PFNGLBLENDFUNCPROC);
    LOAD(glViewport,                 PFNGLVIEWPORTPROC);
    LOAD(glDrawElements,             PFNGLDRAWELEMENTSPROC);
    LOAD(glDrawArrays,               PFNGLDRAWARRAYSPROC);
    LOAD(glPolygonMode,              PFNGLPOLYGONMODEPROC);
    LOAD(glPolygonOffset,            PFNGLPOLYGONOFFSETPROC);
    LOAD(glLineWidth,                PFNGLLINEWIDTHPROC);
    LOAD(glPointSize,                PFNGLPOINTSIZEPROC);
    LOAD(glGetError,                 PFNGLGETERRORPROC);
    LOAD(glGenTextures,              PFNGLGENTEXTURESPROC);
    LOAD(glBindTexture,              PFNGLBINDTEXTUREPROC);
    LOAD(glTexImage2D,               PFNGLTEXIMAGE2DPROC);
    LOAD(glTexParameteri,            PFNGLTEXPARAMETERIPROC);
    LOAD(glGenerateMipmap,           PFNGLGENERATEMIPMAPPROC);
    LOAD(glActiveTexture,            PFNGLACTIVETEXTUREPROC);
    LOAD(glDeleteTextures,           PFNGLDELETETEXTURESPROC);
    LOAD(glGenVertexArrays,          PFNGLGENVERTEXARRAYSPROC);
    LOAD(glBindVertexArray,          PFNGLBINDVERTEXARRAYPROC);
    LOAD(glDeleteVertexArrays,       PFNGLDELETEVERTEXARRAYSPROC);
    LOAD(glGenBuffers,               PFNGLGENBUFFERSPROC);
    LOAD(glBindBuffer,               PFNGLBINDBUFFERPROC);
    LOAD(glBufferData,               PFNGLBUFFERDATAPROC);
    LOAD(glBufferSubData,            PFNGLBUFFERSUBDATAPROC);
    LOAD(glDeleteBuffers,            PFNGLDELETEBUFFERSPROC);
    LOAD(glVertexAttribPointer,      PFNGLVERTEXATTRIBPOINTERPROC);
    LOAD(glEnableVertexAttribArray,  PFNGLENABLEVERTEXATTRIBARRAYPROC);
    LOAD(glDisableVertexAttribArray, PFNGLDISABLEVERTEXATTRIBARRAYPROC);
    LOAD(glCreateShader,             PFNGLCREATESHADERPROC);
    LOAD(glShaderSource,             PFNGLSHADERSOURCEPROC);
    LOAD(glCompileShader,            PFNGLCOMPILESHADERPROC);
    LOAD(glGetShaderiv,              PFNGLGETSHADERIVPROC);
    LOAD(glGetShaderInfoLog,         PFNGLGETSHADERINFOLOGPROC);
    LOAD(glDeleteShader,             PFNGLDELETESHADERPROC);
    LOAD(glCreateProgram,            PFNGLCREATEPROGRAMPROC);
    LOAD(glAttachShader,             PFNGLATTACHSHADERPROC);
    LOAD(glLinkProgram,              PFNGLLINKPROGRAMPROC);
    LOAD(glUseProgram,               PFNGLUSEPROGRAMPROC);
    LOAD(glGetProgramiv,             PFNGLGETPROGRAMIVPROC);
    LOAD(glGetProgramInfoLog,        PFNGLGETPROGRAMINFOLOGPROC);
    LOAD(glDeleteProgram,            PFNGLDELETEPROGRAMPROC);
    LOAD(glGetUniformLocation,       PFNGLGETUNIFORMLOCATIONPROC);
    LOAD(glUniform1i,                PFNGLUNIFORM1IPROC);
    LOAD(glUniform1f,                PFNGLUNIFORM1FPROC);
    LOAD(glUniform2f,                PFNGLUNIFORM2FPROC);
    LOAD(glUniform3f,                PFNGLUNIFORM3FPROC);
    LOAD(glUniform4f,                PFNGLUNIFORM4FPROC);
    LOAD(glUniform1fv,               PFNGLUNIFORM1FVPROC);
    LOAD(glUniform3fv,               PFNGLUNIFORM3FVPROC);
    LOAD(glUniform4fv,               PFNGLUNIFORM4FVPROC);
    LOAD(glUniformMatrix4fv,         PFNGLUNIFORMMATRIX4FVPROC);
    LOAD(glGenFramebuffers,          PFNGLGENFRAMEBUFFERSPROC);
    LOAD(glBindFramebuffer,          PFNGLBINDFRAMEBUFFERPROC);
    LOAD(glFramebufferTexture2D,     PFNGLFRAMEBUFFERTEXTURE2DPROC);
    LOAD(glCheckFramebufferStatus,   PFNGLCHECKFRAMEBUFFERSTATUSPROC);
    LOAD(glDeleteFramebuffers,       PFNGLDELETEFRAMEBUFFERSPROC);
    LOAD(glGenRenderbuffers,         PFNGLGENRENDERBUFFERSPROC);
    LOAD(glBindRenderbuffer,         PFNGLBINDRENDERBUFFERPROC);
    LOAD(glRenderbufferStorage,      PFNGLRENDERBUFFERSTORAGEPROC);
    LOAD(glFramebufferRenderbuffer,  PFNGLFRAMEBUFFERRENDERBUFFERPROC);
    LOAD(glDeleteRenderbuffers,      PFNGLDELETERENDERBUFFERSPROC);
    #undef LOAD
    return glad_glCreateProgram != NULL;
}
