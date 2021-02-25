
#pragma once

#include <gl/glew.h>

namespace spc::opengl
{


static GLint GL4PixelFormatInternal[] = {

        GL_RED,             // ePF_R,
        GL_RG,              // ePF_RG,
        GL_RGB,             // ePF_RGB,
        GL_RGBA,            // ePF_RGBA
        GL_DEPTH_COMPONENT, // ePF_Depth
        GL_DEPTH_STENCIL,   // ePF_DepthStencil
};

static GLint GL4PixelFormatClient[] = {
        GL_RED,   // ePF_R,
        GL_RG,    // ePF_RG,
        GL_RGB,   // ePF_RGB,
        GL_RGBA,  // ePF_RGBA
        GL_DEPTH_COMPONENT, // ePF_Depth
        GL_DEPTH_STENCIL, // ePF_DepthStencil
};


static GLint GL4PixelFormatClientDataType[] = {
        GL_UNSIGNED_BYTE,   // ePF_R,
        GL_UNSIGNED_BYTE,    // ePF_RG,
        GL_UNSIGNED_BYTE,   // ePF_RGB,
        GL_UNSIGNED_BYTE,  // ePF_RGBA
        GL_UNSIGNED_INT,      // ePF_DepthStencil
        GL_UNSIGNED_INT_24_8,  // ePF_DepthStencil

};

}
