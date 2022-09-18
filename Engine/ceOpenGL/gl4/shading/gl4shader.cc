
#include <ceOpenGL/gl4/shading/gl4shader.hh>
#include <ceOpenGL/gl4/gl4exceptions.hh>
#include <GL/glew.h>
#include <stdexcept>

namespace ce::opengl
{


GL4Shader::GL4Shader(eGL4ShaderType type)
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

  m_name = glCreateShader(m_type);
}

GL4Shader::~GL4Shader()
{
  if (m_name)
  {
    glDeleteShader(m_name);
    m_name = 0;
  }
}


void GL4Shader::SetSource(const std::string& source)
{
  m_source = source;
  const GLchar* sources[1];
  sources[0] = m_source.c_str();
  GLint length[1];
  length[0] = (GLint)m_source.length();
  glShaderSource(m_name, 1, sources, length);
}

uint32_t GL4Shader::GetName() const
{
  return m_name;
}

void GL4Shader::Compile()
{
  glCompileShader(m_name);

  GLint state;
  glGetShaderiv(m_name, GL_COMPILE_STATUS, &state);
  if (state == GL_FALSE)
  {
    GLchar buffer[4096];
    GLsizei length;
    glGetShaderInfoLog(m_name, 4096, &length, buffer);
    buffer[length] = '\0';
    std::string msg (buffer);
    throw GL4ShaderCompileException(msg, m_source);
  }
}

}