
#include <ceCore/graphics/pp/pphighpass.hh>

namespace ce
{

PPHighPass::PPHighPass()
{
  DeclareInput(PPImageType::Color, "Color");
  DeclareOutput(PPImageType::Color, "Color");

  m_shader               = ce::AssetManager::Get()->Get<ce::iShader>("${shaders}/pp/high_pass/high_pass.shader");
  m_attribColor          = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
  m_attribHighValue = m_shader ? m_shader->GetShaderAttribute("HighValue") : nullptr;
}

PPHighPass::~PPHighPass()
{
  CE_RELEASE(m_shader);
  m_shader               = nullptr;
  m_attribColor          = nullptr;
}


bool PPHighPass::RefreshOutputTexture(ce::iDevice *device)
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

void PPHighPass::Process(ce::iDevice *device)
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
    if (m_attribHighValue)
    {
      m_attribHighValue->Bind(0.8f);
    }
    device->RenderFullscreen();
  }
}


} // ce