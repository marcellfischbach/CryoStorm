

#pragma once


#ifdef NDEBUG123

#define SPC_GL_ERROR() \
  switch (glGetError()) \
  {\
  case GL_NO_ERROR:\
    break;\
  case GL_INVALID_ENUM:\
    printf("[GL_INVALID_ENUM] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_INVALID_VALUE:\
    printf("[GL_INVALID_VALUE] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_INVALID_OPERATION:\
    printf("[GL_INVALID_OPERATION] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_INVALID_FRAMEBUFFER_OPERATION:\
    printf("[GL_INVALID_FRAMEBUFFER_OPERATION] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_OUT_OF_MEMORY:\
    printf("[GL_OUT_OF_MEMORY] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_STACK_UNDERFLOW:\
    printf("[GL_STACK_UNDERFLOW] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  case GL_STACK_OVERFLOW:\
    printf("[GL_STACK_OVERFLOW] @ %s : %d\n", __FILE__, __LINE__); \
    break;\
  }

#else

#define SPC_GL_ERROR()

#endif
