
#include <ceCore/graphics/pp/ppcombine.hh>

namespace ce
{

PPCombine::PPCombine()
{
  DeclareInput(PPImageType::Color, "Color0");
  DeclareInput(PPImageType::Color, "Color1");
  DeclareOutput(PPImageType::Color, "Color");

  m_shader               = ce::AssetManager::Get()->Get<ce::iShader>("${shaders}/pp/combine/combine.shader");
  m_attribColor0          = m_shader ? m_shader->GetShaderAttribute("Color0") : nullptr;
  m_attribColor1          = m_shader ? m_shader->GetShaderAttribute("Color1") : nullptr;
}

PPCombine::~PPCombine()
{
  CE_RELEASE(m_shader);
  m_shader               = nullptr;
  m_attribColor0          = nullptr;
  m_attribColor1          = nullptr;
}


bool PPCombine::RefreshOutputTexture(ce::iDevice *device)
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

void PPCombine::Process(ce::iDevice *device)
{
  if (m_shader && m_attribColor0 && m_attribColor1 && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, ce::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    if (m_attribColor0)
    {
      ce::eTextureUnit unit = device->BindTexture(m_inputs[0]);
      m_attribColor0->Bind(unit);
    }
    if (m_attribColor1)
    {
      ce::eTextureUnit unit = device->BindTexture(m_inputs[1]);
      m_attribColor1->Bind(unit);
    }
    device->RenderFullscreen();
  }
}


} // ce