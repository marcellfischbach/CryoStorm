
#include <vector>
#include "demopostprocess.hh"
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>

DemoPostProcess::DemoPostProcess()
    : SimplePostProcess()
{
  DeclareInput(cryo::PPImageType::Color, "Color");
  DeclareOutput(cryo::PPImageType::Color, "Color");

  m_shader      = cryo::AssetManager::Get()->Get<cryo::iShader>("${shaders}/pp/demo.shader");
  m_attribColor = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
}


bool DemoPostProcess::RefreshOutputTexture(cryo::iDevice *device)
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
                            cryo::ePF_Depth);
}

void DemoPostProcess::Process(cryo::iDevice *device, cryo::iRenderTarget2D *finalTarget)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_renderTarget);
    device->Clear(false, cryo::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    cryo::eTextureUnit unit = device->BindTexture(m_inputs[0]);
    m_attribColor->Bind(unit);
    device->RenderFullscreen();
  }
}
