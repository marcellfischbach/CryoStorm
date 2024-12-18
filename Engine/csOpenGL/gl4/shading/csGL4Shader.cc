
#include <csOpenGL/gl4/shading/csGL4Shader.hh>
#include <csOpenGL/gl4/csGL4Exceptions.hh>
#include <csOpenGL/csGLError.hh>
#include <GL/glew.h>
#include <stdexcept>

namespace cs::opengl
{


csGL4Shader::csGL4Shader(eGL4ShaderType type)
{
  switch (type)
  {
  case eST_Vertex:
    m_type = GL_VERTEX_SHADER;
    break;
  case eST_TessEval:
    m_type = GL_TESS_EVALUATION_SHADER;
    break;
  case eST_TessControl:
    m_type = GL_TESS_CONTROL_SHADER;
    break;
  case eST_Geometry:
    m_type = GL_GEOMETRY_SHADER;
    break;
  case eST_Fragment:
    m_type = GL_FRAGMENT_SHADER;
    break;
  case eST_Compute:
    m_type = GL_COMPUTE_SHADER;
    break;
  }

  CS_GL_ERROR();
  m_name = glCreateShader(m_type);
  CS_GL_ERROR();
}

csGL4Shader::~csGL4Shader()
{
  if (m_name)
  {
    CS_GL_ERROR();
    glDeleteShader(m_name);
    CS_GL_ERROR();
    m_name = 0;
  }
}


void csGL4Shader::SetSource(const std::string& source)
{
  m_source = source;
  const GLchar* sources[1];
  sources[0] = m_source.c_str();
  GLint length[1];
  length[0] = (GLint)m_source.length();
  CS_GL_ERROR();
  glShaderSource(m_name, 1, sources, length);
  CS_GL_ERROR();
}

uint32_t csGL4Shader::GetName() const
{
  return m_name;
}

void csGL4Shader::Compile()
{
  CS_GL_ERROR();
  glCompileShader(m_name);
  CS_GL_ERROR();

  GLint state;
  glGetShaderiv(m_name, GL_COMPILE_STATUS, &state);
  CS_GL_ERROR();
  if (state == GL_FALSE)
  {
    GLchar buffer[4096];
    GLsizei length;
    glGetShaderInfoLog(m_name, 4096, &length, buffer);
    CS_GL_ERROR();
    buffer[length] = '\0';
    std::string msg (buffer);
    throw csGL4ShaderCompileException(msg, m_source);
  }
}

}