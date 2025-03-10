
#include <csCore/graphics/pp/csPPScaleDown.hh>

namespace cs
{

csPPScaleDown::csPPScaleDown()
{
  DeclareInput(ePPImageType::Color, "Color");
  DeclareOutput(ePPImageType::Color, "Color");

  m_shader               = cs::csAssetManager::Get()->Get<cs::iShader>("shaders/gl4/pp/scale_down/scale_down.shader");
  m_attribColor          = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
  m_attribTextureSizeInv = m_shader ? m_shader->GetShaderAttribute("TextureSizeInv") : nullptr;
}

csPPScaleDown::~csPPScaleDown()
{
  m_shader               = nullptr;
  m_attribColor          = nullptr;
  m_attribTextureSizeInv = nullptr;
}


bool csPPScaleDown::RefreshOutputTexture(cs::iDevice *device)
{
  if (!m_inputs[0])
  {
    return false;
  }

  return UpdateRenderTarget(device,
                            m_inputs[0]->GetWidth() / 2,
                            m_inputs[0]->GetHeight() / 2,
                            true,
                            0,
                            m_inputs[0]->GetFormat(),
                            false,
                            0,
                            cs::ePF_Depth);
}

void csPPScaleDown::Process(iDevice *device, iRenderTarget2D *finalTarget)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, cs::csColor4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    if (m_attribColor)
    {
      cs::eTextureUnit unit = device->BindTexture(m_inputs[0]);
      m_attribColor->Bind(unit);
    }
    if (m_attribTextureSizeInv && m_outputs[0])
    {
      m_attribTextureSizeInv->Bind(1.0f / (float) m_outputs[0]->GetWidth(),
                                   1.0f / (float) m_outputs[0]->GetHeight());
    }
    device->RenderFullscreen();
  }
}


} // ce