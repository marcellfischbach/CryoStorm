
#include <vector>
#include "demopostprocess.hh"
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>

DemoPostProcess::DemoPostProcess()
    : csSimplePostProcess()
{
  DeclareInput(cryo::ePPImageType::Color, "Color");
  DeclareOutput(cryo::ePPImageType::Color, "Color");

  m_shader      = cryo::csAssetManager::Get()->Get<cryo::iShader>("${shaders}/pp/demo.shader");
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
    device->Clear(false, cryo::csColor4f(0, 0, 0, 0), false, 1.0f, false, 0);
    device->SetBlending(false);
    device->SetDepthTest(false);
    device->SetShader(m_shader);
    device->ResetTextures();
    cryo::eTextureUnit unit = device->BindTexture(m_inputs[0]);
    m_attribColor->Bind(unit);
    device->RenderFullscreen();
  }
}
