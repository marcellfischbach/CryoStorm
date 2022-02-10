
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>
#include <ceOpenGL/gl4/gl4texture2darray.hh>
#include <GL/glew.h>

namespace ce::opengl
{

GL4RenderTarget2DArray::GL4RenderTarget2DArray()
    : iRenderTarget2DArray()
      , m_name(0)
      , m_width(0)
      , m_height(0)
      , m_layer(0)
      , m_depthTexture(nullptr)
{
  CE_CLASS_GEN_CONSTR;
  glGenFramebuffers(1, &m_name);
}

GL4RenderTarget2DArray::~GL4RenderTarget2DArray()
{
  glDeleteFramebuffers(1, &m_name);
  m_name = 0;

  CE_RELEASE(m_depthTexture);
  for (auto color : m_colorTextures) {
    color->Release();
  }
  m_colorTextures.clear();
}

void GL4RenderTarget2DArray::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

uint16_t GL4RenderTarget2DArray::GetWidth() const
{
  return m_width;
}


uint16_t GL4RenderTarget2DArray::GetHeight() const
{
  return m_height;
}

uint16_t GL4RenderTarget2DArray::GetLayer() const
{
  return m_layer;
}

bool GL4RenderTarget2DArray::Initialize(uint16_t width, uint16_t height, uint16_t layer)
{
  m_width = width;
  m_height = height;
  m_layer = layer;
  Bind();

  return true;
}

void GL4RenderTarget2DArray::SetDepthTexture(iTexture2DArray *depthTexture)
{
  if (!depthTexture) {
    return;
  }


  GL4Texture2DArray *txt = depthTexture->Query<GL4Texture2DArray>();


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

  CE_SET(m_depthTexture, txt);


  glFramebufferTexture(GL_FRAMEBUFFER,
                       attachment,
                       txt->GetName(),
                       0);

}

void GL4RenderTarget2DArray::AddColorTexture(iTexture2DArray *colorTexture)
{
  if (!colorTexture) {
    return;
  }


  GL4Texture2DArray *txt = colorTexture->Query<GL4Texture2DArray>();
  txt->AddRef();


  GLenum attachment = GL_DEPTH_ATTACHMENT;
  if (colorTexture->GetFormat() == ePF_DepthStencil) {
    return;
  }


  glFramebufferTexture(GL_FRAMEBUFFER,
                       (GLenum) (GL_COLOR_ATTACHMENT0 + m_colorTextures.size()),
                       txt->GetName(),
                       0);
  m_colorTextures.push_back(txt);
}


eTextureType GL4RenderTarget2DArray::GetType() const
{
  return eTT_Texture2DArray;
}

bool GL4RenderTarget2DArray::Compile()
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

std::string GL4RenderTarget2DArray::GetCompileLog() const
{
  return m_log;
}

iTexture2DArray *GL4RenderTarget2DArray::GetDepthTexture()
{
  return m_depthTexture;
}

const iTexture2DArray *GL4RenderTarget2DArray::GetDepthTexture() const
{
  return m_depthTexture;
}


Size GL4RenderTarget2DArray::GetNumberOfColorTextures() const
{
  return m_colorTextures.size();
}

iTexture2DArray *GL4RenderTarget2DArray::GetColorTexture(Size idx)
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


const iTexture2DArray *GL4RenderTarget2DArray::GetColorTexture(Size idx) const
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


}