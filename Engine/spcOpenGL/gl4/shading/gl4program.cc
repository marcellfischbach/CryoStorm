#include <spcOpenGL/gl4/shading/gl4program.hh>
#include <spcOpenGL/gl4/shading/gl4shader.hh>
#include <spcOpenGL/gl4/gl4exceptions.hh>
#include <GL/glew.h>


namespace spc
{


GL4Program::GL4Program()
{
  SPC_CLASS_GEN_CONSTR;
  m_name = glCreateProgram();
}

GL4Program::~GL4Program()
{
  if (m_name)
  {
    glDeleteProgram(m_name);
    m_name = 0;
  }

  for (GL4Shader* shader : m_shaders)
  {
    shader->Release();
  }
  m_shaders.clear();
}

void GL4Program::AttachShader(GL4Shader* shader)
{
  if (!shader)
  {
    return;
  }
  if (std::find(m_shaders.begin(), m_shaders.end(), shader) != m_shaders.end())
  {
    return;
  }

  shader->AddRef();
  m_shaders.push_back(shader);

  glAttachShader(m_name, shader->GetName());
}

void GL4Program::DetachShader(GL4Shader* shader)
{
  if (!shader)
  {
    return;
  }
  auto it = std::find(m_shaders.begin(), m_shaders.end(), shader);
  if (it == m_shaders.end())
  {
    return;
  }


  glDetachShader(m_name, shader->GetName());
  m_shaders.erase(it);
  shader->Release();
}

void GL4Program::Link()
{
  glLinkProgram(m_name);
  GLint state;
  glGetProgramiv(m_name, GL_LINK_STATUS, &state);
  if (state == GL_FALSE)
  {
    GLchar buffer[4096];
    GLsizei length;
    glGetProgramInfoLog(m_name, 4096, &length, buffer);
    buffer[length] = '\0';
    throw GL4ProgramLinkException(std::string(buffer));
  }
}

UInt32 GL4Program::GetName() const
{
  return m_name;
}


UInt32 GL4Program::RegisterAttribute(const std::string& attributeName)
{
  return 0;
}

UInt32 GL4Program::GetAttributeId(const std::string& attributeName)
{
  return 0;
}


iShaderAttribute* GL4Program::GetShaderAttribute(UInt32 id)
{
  return nullptr;
}


iShaderAttribute* GL4Program::GetShaderAttribute(const std::string& attributeName)
{
  return nullptr;
}


iShaderAttribute* GL4Program::GetShaderAttribute(eRenderAttribute renderAttribute)
{
  return nullptr;
}


}