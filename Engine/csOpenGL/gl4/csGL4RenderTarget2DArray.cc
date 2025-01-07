
#include <csOpenGL/gl4/csGL4RenderTarget2DArray.hh>
#include <csOpenGL/gl4/csGL4Texture2DArray.hh>
#include <csOpenGL/csGLError.hh>
#include <GL/glew.h>

namespace cs::opengl
{

csGL4RenderTarget2DArray::csGL4RenderTarget2DArray()
    : iRenderTarget2DArray()
      , m_name(0)
      , m_width(0)
      , m_height(0)
      , m_layer(0)
      , m_depthTexture(nullptr)
{

  CS_GL_ERROR();
  CS_GL_ERROR();
  glGenFramebuffers(1, &m_name);
}

csGL4RenderTarget2DArray::~csGL4RenderTarget2DArray()
{
  CS_GL_ERROR();
  glDeleteFramebuffers(1, &m_name);
  CS_GL_ERROR();
  m_name = 0;

  CS_RELEASE(m_depthTexture);
  for (auto color : m_colorTextures) {
    color->Release();
  }
  m_colorTextures.clear();
}

void csGL4RenderTarget2DArray::Bind()
{
  CS_GL_ERROR();
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_GL_ERROR();
}

uint16_t csGL4RenderTarget2DArray::GetWidth() const
{
  return m_width;
}


uint16_t csGL4RenderTarget2DArray::GetHeight() const
{
  return m_height;
}

uint16_t csGL4RenderTarget2DArray::GetLayer() const
{
  return m_layer;
}

bool csGL4RenderTarget2DArray::Initialize(uint16_t width, uint16_t height, uint16_t layer)
{
  m_width = width;
  m_height = height;
  m_layer = layer;
  Bind();

  return true;
}

void csGL4RenderTarget2DArray::SetDepthTexture(iTexture2DArray *depthTexture)
{
  if (!depthTexture) {
    return;
  }


  csGL4Texture2DArray *txt = depthTexture->Query<csGL4Texture2DArray>();


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


  CS_GL_ERROR();
  glFramebufferTexture(GL_FRAMEBUFFER,
                       attachment,
                       txt->GetName(),
                       0);
  CS_GL_ERROR();

}

void csGL4RenderTarget2DArray::AddColorTexture(iTexture2DArray *colorTexture)
{
  if (!colorTexture) {
    return;
  }


  csGL4Texture2DArray *txt = colorTexture->Query<csGL4Texture2DArray>();
  txt->AddRef();


  GLenum attachment = GL_DEPTH_ATTACHMENT;
  if (colorTexture->GetFormat() == ePF_DepthStencil) {
    return;
  }


  CS_GL_ERROR();
  glFramebufferTexture(GL_FRAMEBUFFER,
                       (GLenum) (GL_COLOR_ATTACHMENT0 + m_colorTextures.size()),
                       txt->GetName(),
                       0);
  CS_GL_ERROR();
  m_colorTextures.push_back(txt);
}


eTextureType csGL4RenderTarget2DArray::GetType() const
{
  return eTextureType::Texture2DArray;
}

bool csGL4RenderTarget2DArray::Compile()
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

std::string csGL4RenderTarget2DArray::GetCompileLog() const
{
  return m_log;
}

iTexture2DArray *csGL4RenderTarget2DArray::GetDepthTexture()
{
  return m_depthTexture;
}

const iTexture2DArray *csGL4RenderTarget2DArray::GetDepthTexture() const
{
  return m_depthTexture;
}


Size csGL4RenderTarget2DArray::GetNumberOfColorTextures() const
{
  return m_colorTextures.size();
}

iTexture2DArray *csGL4RenderTarget2DArray::GetColorTexture(Size idx)
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


const iTexture2DArray *csGL4RenderTarget2DArray::GetColorTexture(Size idx) const
{
  if (idx >= m_colorTextures.size()) {
    return nullptr;
  }
  return m_colorTextures[idx];
}


}