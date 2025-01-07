#include <csOpenGL/gl4/shading/csGL4Program.hh>
#include <csOpenGL/gl4/shading/csGL4Shader.hh>
#include <csOpenGL/gl4/shading/csGL4ShaderAttribute.hh>
#include <csOpenGL/gl4/shading/csGL4ShaderAttributeNull.hh>
#include <csOpenGL/gl4/csGL4Exceptions.hh>
#include <csOpenGL/csGLError.hh>
#include <GL/glew.h>


namespace cs::opengl
{


csGL4Program::csGL4Program()
{

  CS_GL_ERROR();
  m_name = glCreateProgram();
  CS_GL_ERROR();
}

csGL4Program::~csGL4Program()
{
  if (m_name)
  {
    CS_GL_ERROR();
    glDeleteProgram(m_name);
    CS_GL_ERROR();
    m_name = 0;
  }

  for (csGL4Shader *shader: m_shaders)
  {
    shader->Release();
  }
  m_shaders.clear();
}

void csGL4Program::AttachShader(csGL4Shader *shader)
{
  if (!shader)
  {
    return;
  }
  if (std::ranges::find(m_shaders.begin(), m_shaders.end(), shader) != m_shaders.end())
  {
    return;
  }

  shader->AddRef();
  m_shaders.push_back(shader);

  CS_GL_ERROR();
  glAttachShader(m_name, shader->GetName());
  CS_GL_ERROR();
}

void csGL4Program::DetachShader(csGL4Shader *shader)
{
  if (!shader)
  {
    return;
  }
  auto it = std::ranges::find(m_shaders.begin(), m_shaders.end(), shader);
  if (it == m_shaders.end())
  {
    return;
  }


  CS_GL_ERROR();
  glDetachShader(m_name, shader->GetName());
  CS_GL_ERROR();
  m_shaders.erase(it);
  shader->Release();
}

void csGL4Program::Link()
{
  CS_GL_ERROR();
  glLinkProgram(m_name);
  GLint state;
  CS_GL_ERROR();
  glGetProgramiv(m_name, GL_LINK_STATUS, &state);
  CS_GL_ERROR();
  if (state == GL_FALSE)
  {
    GLchar  buffer[4096];
    GLsizei length;
    glGetProgramInfoLog(m_name, 4096, &length, buffer);
    CS_GL_ERROR();
    buffer[length] = '\0';
    throw csGL4ProgramLinkException(std::string(buffer));
  }

  m_attributes.clear();
  RegisterRenderAttributes();
}

uint32_t csGL4Program::GetName() const
{
  return m_name;
}


void csGL4Program::RegisterRenderAttributes()
{
  RegisterAttribute("ModelMatrix");
  RegisterAttribute("ViewMatrix");
  RegisterAttribute("ProjectionMatrix");
  RegisterAttribute("ModelViewMatrix");
  RegisterAttribute("ViewProjectionMatrix");
  RegisterAttribute("ModelViewProjectionMatrix");
  RegisterAttribute("ModelMatrixInv");
  RegisterAttribute("ViewMatrixInv");
  RegisterAttribute("ProjectionMatrixInv");
  RegisterAttribute("ModelViewMatrixInv");
  RegisterAttribute("ViewProjectionMatrixInv");
  RegisterAttribute("ModelViewProjectionMatrixInv");

  RegisterAttribute("Random");

  RegisterAttribute("ShadowMapViewMatrix");
  RegisterAttribute("ShadowMapProjectionMatrix");
  RegisterAttribute("ShadowMapViewProjectionMatrix");

  RegisterAttribute("RenderLayer");

  RegisterAttribute("LightColor");
  RegisterAttribute("LightVector");
  RegisterAttribute("LightRange");
  RegisterAttribute("LightCount");
  RegisterAttribute("LightCastShadow");
  RegisterAttribute("LightShadowMap");

  RegisterAttribute("PointLightShadowMapMappingBias");
  RegisterAttribute("PointLightShadowMapColor");
  RegisterAttribute("PointLightShadowMapDepth");

  RegisterAttribute("DirectionalLightShadowMapSplitLayers");
  RegisterAttribute("DirectionalLightShadowMapViewProjectionMatrix");
  RegisterAttribute("DirectionalLightShadowMapDepth");
  RegisterAttribute("DirectionalLightShadowMapColor");
  RegisterAttribute("DirectionalLightShadowMapLayersBias");

  RegisterAttribute("SkeletonMatrices");


//  RegisterAttribute("DirectionalLightShadowMapLayersBias");
//  RegisterAttribute("DirectionalLightShadowMapMatrices");
//  RegisterAttribute("DirectionalLightShadowMapColor");
//  RegisterAttribute("DirectionalLightShadowMapDepth");

}

Size csGL4Program::RegisterAttribute(const std::string &attributeName)
{
  for (uint32_t i = 0; i < m_attributes.size(); i++)
  {
    iShaderAttribute *attribute = m_attributes[i];
    if (attribute && attribute->GetName() == attributeName)
    {
      return i;
    }
  }

  std::string      locationName = "cs_" + attributeName;
  GLint            location     = glGetUniformLocation(m_name, locationName.c_str());
  iShaderAttribute *attribute   = nullptr;
  if (location != -1)
  {
    attribute = new csGL4ShaderAttribute(location, attributeName);
  }
  m_attributes.push_back(attribute);
  return m_attributes.size() - 1;
}

Size csGL4Program::GetAttributeId(const std::string &attributeName)
{
  for (uint32_t i = 0; i < m_attributes.size(); i++)
  {
    iShaderAttribute *attribute = m_attributes[i];

    if (attribute && attribute->GetName() == attributeName)
    {
      return i;
    }
  }

  return ~0x00;
}


iShaderAttribute *csGL4Program::GetShaderAttribute(uint32_t id)
{
  if (id >= m_attributes.size())
  {
    return nullptr;
  }
  return m_attributes[id];
}


iShaderAttribute *csGL4Program::GetShaderAttribute(const std::string &attributeName)
{
  for (auto attribute: m_attributes)
  {
    if (attribute && attribute->GetName() == attributeName)
    {
      return attribute;
    }
  }
  return nullptr;
}


iShaderAttribute *csGL4Program::GetShaderAttribute(eShaderAttribute shaderAttribute)
{
  return m_attributes[shaderAttribute];
}


}