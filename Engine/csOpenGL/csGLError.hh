

#pragma once


#ifdef _DEBUG

#include <GL/glew.h>

static void csGLErrorCallError(GLenum error, const char *file, int line)
{
  if (error != GL_NO_ERROR)
  {
    switch (error)
    {
      case GL_NO_ERROR:
        break;
      case GL_INVALID_ENUM:
        printf("[GL_INVALID_ENUM] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_INVALID_VALUE:
        printf("[GL_INVALID_VALUE] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_INVALID_OPERATION:
        printf("[GL_INVALID_OPERATION] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        printf("[GL_INVALID_FRAMEBUFFER_OPERATION] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_OUT_OF_MEMORY:
        printf("[GL_OUT_OF_MEMORY] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_STACK_UNDERFLOW:
        printf("[GL_STACK_UNDERFLOW] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      case GL_STACK_OVERFLOW:
        printf("[GL_STACK_OVERFLOW] @ %s : %d\n", file, line);
        fflush(stdout);
        break;
      default:
        printf("[Unknown: 0x%08x] @ %s : %d\n", error, file, line);
        fflush(stdout);
        break;
    }
  }
}

#define CS_GL_ERROR() \
  {                   \
  GLenum error = glGetError(); \
  csGLErrorCallError(error, __FILE__, __LINE__);\
  }

#else

#define CS_GL_ERROR()

#endif
