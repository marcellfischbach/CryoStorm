
#include <ceCore/graphics/pp/ppblurh.hh>

namespace ce
{

PPBlurH::PPBlurH()
{
  DeclareInput(PPImageType::Color, "Color");
  DeclareOutput(PPImageType::Color, "Color");

  m_shader               = ce::AssetManager::Get()->Get<ce::iShader>("${shaders}/pp/blur_h/blur_h.shader");
  m_attribColor          = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
  m_attribTextureSizeInv = m_shader ? m_shader->GetShaderAttribute("TextureSizeInv") : nullptr;
  m_attribSampleCount = m_shader ? m_shader->GetShaderAttribute("SampleCount") : nullptr;
  m_attribSampleScale = m_shader ? m_shader->GetShaderAttribute("SampleScale") : nullptr;
}

PPBlurH::~PPBlurH()
{
  CE_RELEASE(m_shader);
  m_shader               = nullptr;
  m_attribColor          = nullptr;
  m_attribTextureSizeInv = nullptr;
}


bool PPBlurH::RefreshOutputTexture(ce::iDevice *device)
{
  if (!m_inputs[0])
  {
    return false;
  }

  return UpdateRenderTarget(device,
                            m_inputs[0]->GetWidth(),
                            m_inputs[0]->GetHeight(),
                            true,
                            0,
                            m_inputs[0]->GetFormat(),
                            false,
                            0,
                            ce::ePF_Depth);
}

void PPBlurH::Process(ce::iDevice *device)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, ce::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    if (m_attribColor)
    {
      ce::eTextureUnit unit = device->BindTexture(m_inputs[0]);
      m_attribColor->Bind(unit);
    }
    if (m_attribTextureSizeInv && m_outputs[0])
    {
      m_attribTextureSizeInv->Bind(1.0f / (float) m_outputs[0]->GetWidth());
    }
    if (m_attribSampleScale)
    {
      m_attribSampleScale->Bind(10.0f);
    }
    if (m_attribSampleCount)
    {
      m_attribSampleCount->Bind(30);
    }
    device->RenderFullscreen();
  }
}


} // ce