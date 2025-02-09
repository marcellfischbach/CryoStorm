
#pragma once

#include <gl/glew.h>

namespace cs::opengl
{


static GLint csGL4PixelFormatInternal[] = {

    GL_RED,             // ePF_R,
    GL_RG,              // ePF_RG,
    GL_RGB,             // ePF_RGB,
    GL_RGBA,            // ePF_RGBA
    GL_RGBA,            // ePF_RGB10A2
    GL_DEPTH_COMPONENT, // ePF_Depth
    GL_DEPTH_STENCIL,   // ePF_DepthStencil
    GL_RG,              // ePF_RG16F
};

static GLint csGL4PixelFormatSizedInternal[] = {

    GL_R8,                // ePF_R,
    GL_RG8,               // ePF_RG,
    GL_RGB8,              // ePF_RGB,
    GL_RGBA8,             // ePF_RGBA
    GL_RGB10_A2,          // ePF_RGB10A2
    GL_DEPTH_COMPONENT32, // ePF_Depth
    GL_DEPTH24_STENCIL8,  // ePF_DepthStencil
    GL_RG16F,             // ePF_RG16F
};

static GLint csGL4PixelFormatClient[] = {
    GL_RED,   // ePF_R,
    GL_RG,    // ePF_RG,
    GL_RGB,   // ePF_RGB,
    GL_RGBA,  // ePF_RGBA
    GL_RGBA,  // ePF_RGBA
    GL_DEPTH_COMPONENT, // ePF_Depth
    GL_DEPTH_STENCIL, // ePF_DepthStencil
    GL_RG,    // ePF_RG16F,
};


static GLint csGL4PixelFormatClientDataType[] = {
    GL_UNSIGNED_BYTE,   // ePF_R,
    GL_UNSIGNED_BYTE,    // ePF_RG,
    GL_UNSIGNED_BYTE,   // ePF_RGB,
    GL_UNSIGNED_BYTE,   // ePF_RGB,
    GL_UNSIGNED_BYTE,  // ePF_RGBA
    GL_UNSIGNED_INT,      // ePF_DepthStencil
    GL_UNSIGNED_INT_24_8,  // ePF_DepthStencil
    GL_FLOAT,             // ePF_RG16F
};

}
