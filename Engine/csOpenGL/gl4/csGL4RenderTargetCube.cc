
#include <csOpenGL/gl4/csGL4RenderTargetCube.hh>
#include <csOpenGL/gl4/csGL4TextureCube.hh>
#include <csOpenGL/csGLError.hh>
#include <GL/glew.h>

namespace cs::opengl
{

csGL4RenderTargetCube::csGL4RenderTargetCube()
  : iRenderTargetCube()
  , m_name(0)
  , m_size(0)
  , m_depthTexture(nullptr)
{

  CS_GL_ERROR();
  glGenFramebuffers(1, &m_name);
  CS_GL_ERROR();
}

csGL4RenderTargetCube::~csGL4RenderTargetCube()
{
  if (!m_external)
  {
    CS_GL_ERROR();
    glDeleteFramebuffers(1, &m_name);
    CS_GL_ERROR();
  }
  m_name = 0;

  m_depthTexture = nullptr;
  m_colorTextures.clear();
}

void csGL4RenderTargetCube::Bind()
{
  CS_GL_ERROR();
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_GL_ERROR();
}

uint16_t csGL4RenderTargetCube::GetSize() const
{
  return m_size;
}


bool csGL4RenderTargetCube::Initialize(uint16_t size)
{
  m_size = size;
  m_external = false;
  Bind();

  return true;
}

bool csGL4RenderTargetCube::Initialize(uint32_t name, uint16_t size)
{
  m_size = size;
  m_name = name;
  m_external = true;
  Bind();
  return true;
}

void csGL4RenderTargetCube::SetDepthTexture(iTextureCube* depthTexture)
{
  if (!depthTexture) {
    return;
  }


  csGL4TextureCube * txt = depthTexture->Query<csGL4TextureCube>();


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

  m_depthTexture =  txt;


  CS_GL_ERROR();
  glFramebufferTexture(GL_FRAMEBUFFER,
    attachment,
    txt->GetName(),
    0);
  CS_GL_ERROR();

}


void csGL4RenderTargetCube::AddColorTexture(iTextureCube* colorTexture)
{
  if (!colorTexture) {
    return;
  }


  csGL4TextureCube * txt = colorTexture->Query<csGL4TextureCube>();
  txt->AddRef();


  if (colorTexture->GetFormat() == ePF_DepthStencil) {
    return;
  }


  CS_GL_ERROR();
  glFramebufferTexture(GL_FRAMEBUFFER,
    (GLenum)(GL_COLOR_ATTACHMENT0 + m_colorTextures.size()),
    txt->GetName(),
    0);
  CS_GL_ERROR();
  m_colorTextures.push_back(txt);
}


eTextureType csGL4RenderTargetCube::GetType() const
{
  return eTextureType::TextureCube;
}

bool csGL4RenderTargetCube::Compile()
{
  GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  CS_GL_ERROR();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CS_GL_ERROR();
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

std::string csGL4RenderTargetCube::GetCompileLog() const
{
  return m_log;
}

iTextureCube* csGL4RenderTargetCube::GetDepthTexture()
{
  return m_depthTexture;
}

const iTextureCube* csGL4RenderTargetCube::GetDepthTexture() const
{
  return m_depthTexture;
}

Size csGL4RenderTargetCube::GetNumberOfColorTextures() const
{
  return m_colorTextures.size();
}

iTextureCube* csGL4RenderTargetCube::GetColorTexture(Size idx)
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


const iTextureCube* csGL4RenderTargetCube::GetColorTexture(Size idx) const
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


}