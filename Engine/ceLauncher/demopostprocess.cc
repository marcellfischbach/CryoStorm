
#include <vector>
#include <ceLauncher/demopostprocess.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>

DemoPostProcess::DemoPostProcess()
    : SimplePostProcess()
{
  DeclareInput(ce::PPImageType::Color, "Color");
  DeclareOutput(ce::PPImageType::Color, "Color");

  m_shader      = ce::AssetManager::Get()->Get<ce::iShader>("${shaders}/pp/demo.shader");
  m_attribColor = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
}


bool DemoPostProcess::RefreshOutputTexture(ce::iDevice *device)
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

void DemoPostProcess::Process(ce::iDevice *device, ce::iRenderTarget2D *finalTarget)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, ce::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    ce::eTextureUnit unit = device->BindTexture(m_inputs[0]);
    m_attribColor->Bind(unit);
    device->RenderFullscreen();
  }
}
