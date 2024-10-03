
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
#include <ceOpenGL/gl4/gl4texturecube.hh>
#include <GL/glew.h>

namespace ce::opengl
{

GL4RenderTargetCube::GL4RenderTargetCube()
  : iRenderTargetCube()
  , m_name(0)
  , m_size(0)
  , m_depthTexture(nullptr)
{
  CS_CLASS_GEN_CONSTR;
  glGenFramebuffers(1, &m_name);
}

GL4RenderTargetCube::~GL4RenderTargetCube()
{
  if (!m_external)
  {
    glDeleteFramebuffers(1, &m_name);
  }
  m_name = 0;

  CS_RELEASE(m_depthTexture);
  for (auto color : m_colorTextures)
  {
    color->Release();
  }
  m_colorTextures.clear();
}

void GL4RenderTargetCube::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

uint16_t GL4RenderTargetCube::GetSize() const
{
  return m_size;
}


bool GL4RenderTargetCube::Initialize(uint16_t size)
{
  m_size = size;
  m_external = false;
  Bind();

  return true;
}

bool GL4RenderTargetCube::Initialize(uint32_t name, uint16_t size)
{
  m_size = size;
  m_name = name;
  m_external = true;
  Bind();
  return true;
}

void GL4RenderTargetCube::SetDepthTexture(iTextureCube* depthTexture)
{
  if (!depthTexture) {
    return;
  }


  GL4TextureCube* txt = depthTexture->Query<GL4TextureCube>();


  GLenum attachment = 0;
  switch (depthTexture->GetFormat()) {
  case ePF_Depth:
    attachment = GL_DEPTH_ATTACHMENT;
    break;
  case ePF_DepthStencil:
    attachment = GL_DEPTH_STENCIL_ATTACHMENT;
    break;
  default:
    return;
  }

  CS_SET(m_depthTexture, txt);


  glFramebufferTexture(GL_FRAMEBUFFER,
    attachment,
    txt->GetName(),
    0);

}


void GL4RenderTargetCube::AddColorTexture(iTextureCube* colorTexture)
{
  if (!colorTexture) {
    return;
  }


  GL4TextureCube* txt = colorTexture->Query<GL4TextureCube>();
  txt->AddRef();


  if (colorTexture->GetFormat() == ePF_DepthStencil) {
    return;
  }


  glFramebufferTexture(GL_FRAMEBUFFER,
    (GLenum)(GL_COLOR_ATTACHMENT0 + m_colorTextures.size()),
    txt->GetName(),
    0);
  m_colorTextures.push_back(txt);
}


eTextureType GL4RenderTargetCube::GetType() const
{
  return eTextureType::TextureCube;
}

bool GL4RenderTargetCube::Compile()
{
  GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  switch (res) {
  case GL_FRAMEBUFFER_COMPLETE:
    m_log = "Complete";
    return true;
  case GL_FRAMEBUFFER_UNDEFINED:
    m_log = "Framebuffer undefined";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    m_log = "Framebuffer incomplete attachment";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    m_log = "Framebuffer incomplete missing attachment";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    m_log = "Framebuffer incomplete draw buffer";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
    m_log = "Framebuffer incomplete read buffer";
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED:
    m_log = "Framebuffer unsupported";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
    m_log = "Framebuffer incomplete multisample";
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
    m_log = "Framebuffer incomplete layer targets";
    break;
  }
  return false;
}

std::string GL4RenderTargetCube::GetCompileLog() const
{
  return m_log;
}

iTextureCube* GL4RenderTargetCube::GetDepthTexture()
{
  return m_depthTexture;
}

const iTextureCube* GL4RenderTargetCube::GetDepthTexture() const
{
  return m_depthTexture;
}

Size GL4RenderTargetCube::GetNumberOfColorTextures() const
{
  return m_colorTextures.size();
}

iTextureCube* GL4RenderTargetCube::GetColorTexture(Size idx)
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


const iTextureCube* GL4RenderTargetCube::GetColorTexture(Size idx) const
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


}