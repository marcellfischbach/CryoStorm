
#pragma once

#include <gl/glew.h>


namespace ce::opengl
{

static GLint GL4CompareFunc[] = {

  GL_LESS,      // eCF_Less,
  GL_LEQUAL,    // eCF_LessOrEqual,
  GL_GREATER,   // eCF_Greater,
  GL_GEQUAL,    // eCF_GreaterOrEqual,
  GL_EQUAL,     // eCF_Equal,
  GL_NOTEQUAL,  // eCF_NotEqual,
  GL_ALWAYS,    // eCF_Always,
  GL_NEVER,     // eCF_Never,
};


};

