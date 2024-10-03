
#include <ceCore/graphics/pp/pphighpass.hh>

namespace cryo
{

PPHighPass::PPHighPass(float highValue)
: m_highValue(highValue)
{
  DeclareInput(PPImageType::Color, "Color");
  DeclareOutput(PPImageType::Color, "Color");

  m_shader               = cryo::AssetManager::Get()->Get<cryo::iShader>("${shaders}/pp/high_pass/high_pass.shader");
  m_attribColor          = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
  m_attribHighValue = m_shader ? m_shader->GetShaderAttribute("HighValue") : nullptr;
}

PPHighPass::~PPHighPass()
{
  CS_RELEASE(m_shader);
  m_shader               = nullptr;
  m_attribColor          = nullptr;
}



bool PPHighPass::RefreshOutputTexture(cryo::iDevice *device, iRenderTarget2D *finalTarget)
{
  if (finalTarget)
  {
    CS_SET(m_renderTarget, finalTarget);
    return finalTarget;
  }


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
                            cryo::ePF_Depth);
}

void PPHighPass::Process(iDevice *device, iRenderTarget2D *finalTarget)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device, finalTarget))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, cryo::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    if (m_attribColor)
    {
      cryo::eTextureUnit unit = device->BindTexture(m_inputs[0]);
      m_attribColor->Bind(unit);
    }
    if (m_attribHighValue)
    {
      m_attribHighValue->Bind(m_highValue);
    }
    device->RenderFullscreen();
  }
}


} // ce