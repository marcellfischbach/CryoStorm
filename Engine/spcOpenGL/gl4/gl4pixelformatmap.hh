
#pragma once

#include <gl/glew.h>

namespace spc
{


GLint GL4PixelFormatInternal[] = {

  GL_RED,   // ePF_R,
    GL_RG,    // ePF_RG,
    GL_RGB,   // ePF_RGB,
    GL_RGBA,  // ePF_RGBA
};

GLint GL4PixelFormatClient[] = {
  GL_RED,   // ePF_R,
    GL_RG,    // ePF_RG,
    GL_RGB,   // ePF_RGB,
    GL_RGBA,  // ePF_RGBA
};


GLint GL4PixelFormatClientDataType[] = {
  GL_UNSIGNED_BYTE,   // ePF_R,
    GL_UNSIGNED_BYTE,    // ePF_RG,
    GL_UNSIGNED_BYTE,   // ePF_RGB,
    GL_UNSIGNED_BYTE,  // ePF_RGBA

};

}
