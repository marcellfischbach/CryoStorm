
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4texture2d.hh>
#include <GL/glew.h>

namespace ce::opengl
{

GL4RenderTarget2D::GL4RenderTarget2D()
    : iRenderTarget2D(), m_name(0), m_width(0), m_height(0), m_depthBuffer(0), m_depthTexture(nullptr)
{
  CE_CLASS_GEN_CONSTR;
  glGenFramebuffers(1, &m_name);
}

GL4RenderTarget2D::~GL4RenderTarget2D()
{
  glDeleteFramebuffers(1, &m_name);
  m_name = 0;

  CE_RELEASE(m_depthTexture);
  if (m_depthBuffer)
  {
    glDeleteRenderbuffers(1, &m_depthBuffer);
    m_depthBuffer = 0;
  }
  for (auto color: m_colorTextures)
  {
    color->Release();
  }
  m_colorTextures.clear();
}

void GL4RenderTarget2D::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

uint16_t GL4RenderTarget2D::GetWidth() const
{
  return m_width;
}


uint16_t GL4RenderTarget2D::GetHeight() const
{
  return m_height;
}

bool GL4RenderTarget2D::Initialize(uint16_t width, uint16_t height)
{
  m_width = width;
  m_height = height;
  Bind();

  return true;
}

void GL4RenderTarget2D::SetDepthTexture(iTexture2D *depthTexture)
{
  if (!depthTexture)
  {
    return;
  }


  GL4Texture2D *txt = depthTexture->Query<GL4Texture2D>();


  GLenum attachment;
  switch (depthTexture->GetFormat())
  {
    case ePF_Depth:
      attachment = GL_DEPTH_ATTACHMENT;
      break;
    case ePF_DepthStencil:
      attachment = GL_DEPTH_STENCIL_ATTACHMENT;
      break;
    default:
      return;
  }

  CE_SET(m_depthTexture, txt);


  glFramebufferTexture(GL_FRAMEBUFFER,
                       attachment,
                       txt->GetName(),
                       0);

}

void GL4RenderTarget2D::SetDepthBuffer(ePixelFormat format)
{

  GLenum attachment = 0;
  GLenum internalFormat = 0;
  switch (format)
  {
    case ePF_Depth:
      internalFormat = GL_DEPTH_COMPONENT;
      attachment = GL_DEPTH_ATTACHMENT;
      break;
    case ePF_DepthStencil:
      internalFormat = GL_DEPTH_STENCIL;
      attachment = GL_DEPTH_STENCIL_ATTACHMENT;
      break;
    default:
      return;
  }
  glGenRenderbuffers(1, &m_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_depthBuffer);
}

void GL4RenderTarget2D::AddColorTexture(iTexture2D *colorTexture)
{
  if (!colorTexture)
  {
    return;
  }


  GL4Texture2D *txt = colorTexture->Query<GL4Texture2D>();
  txt->AddRef();


  GLenum attachment = GL_DEPTH_ATTACHMENT;
  if (colorTexture->GetFormat() == ePF_DepthStencil)
  {
    return;
  }


  glFramebufferTexture(GL_FRAMEBUFFER,
                       (GLenum) (GL_COLOR_ATTACHMENT0 + m_colorTextures.size()),
                       txt->GetName(),
                       0);
  m_colorTextures.push_back(txt);
}


eTextureType GL4RenderTarget2D::GetType() const
{
  return eTextureType::Texture2D;
}

bool GL4RenderTarget2D::Compile()
{
  GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  switch (res)
  {
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

std::string GL4RenderTarget2D::GetCompileLog() const
{
  return m_log;
}

iTexture2D *GL4RenderTarget2D::GetDepthTexture()
{
  return m_depthTexture;
}

const iTexture2D *GL4RenderTarget2D::GetDepthTexture() const
{
  return m_depthTexture;
}


Size GL4RenderTarget2D::GetNumberOfColorTextures() const
{
  return m_colorTextures.size();
}

iTexture2D *GL4RenderTarget2D::GetColorTexture(Size idx)
{
  if (idx >= m_colorTextures.size())
  {
    return nullptr;
  }
  return m_colorTextures[idx];
}


const iTexture2D *GL4RenderTarget2D::GetColorTexture(Size idx) const
{
  if (idx >= m_colorTextures.size())
  {
    return nullptr;
  }
  return m_colorTextures[idx];
}


}