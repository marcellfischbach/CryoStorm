
#include <vector>
#include <ceLauncher/demopostprocess.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>

DemoPostProcess::DemoPostProcess()
{
  m_inputDefinitions.push_back({ 0, ce::PPInputSource::LastStep, ce::PPImageType::Color });
  m_outputDefinitions.push_back({ 0, ce::PPImageType::Color });

  m_shader = ce::AssetManager::Get()->Get<ce::iShader>("${shaders}/pp/demo.shader");
  m_attribColor = m_shader ? m_shader->GetShaderAttribute("Color") : nullptr;
}

const std::vector <ce::PPInputDefinition> &DemoPostProcess::GetInputDefinitions() const
{
  return m_inputDefinitions;
}
const std::vector <ce::PPOutputDefinition> &DemoPostProcess::GetOutputDefinitions() const
{
  return m_outputDefinitions;
}
void DemoPostProcess::SetInput(size_t idx, ce::iTexture2D *texture)
{
  if (idx == 0)
  {
    CE_SET(m_inputTexture, texture);
  }
}
ce::iTexture2D *DemoPostProcess::GetOutput(size_t idx) const
{
  return idx == 0  && m_outputTarget ? m_outputTarget->GetColorTexture(0) : nullptr;
}


bool DemoPostProcess::RefreshOutputTexture(ce::iDevice *device)
{
  if (!m_inputTexture)
  {
    return false;
  }

  if (m_outputTarget
      && m_inputTexture->GetWidth() == m_outputTarget->GetWidth()
      && m_inputTexture->GetHeight() == m_outputTarget->GetHeight()
      )
  {
    return true;
  }

  if (m_outputTarget)
  {
    CE_RELEASE(m_outputTarget);
    m_outputTarget = nullptr;
  }

  ce::iTexture2D::Descriptor colorDesc {
    m_inputTexture->GetFormat(),
    (uint16_t)m_inputTexture->GetWidth(),
    (uint16_t)m_inputTexture->GetHeight(),
    false,
    1
  };
  ce::iTexture2D *colorTexture = device->CreateTexture(colorDesc);

  ce::iRenderTarget2D::Descriptor desc { (uint16_t)m_inputTexture->GetWidth(),
                                         (uint16_t)m_inputTexture->GetHeight() };
  m_outputTarget = device->CreateRenderTarget(desc);
  m_outputTarget->AddColorTexture(colorTexture);
  return m_outputTarget->Compile();
}

void DemoPostProcess::Process(ce::iDevice *device)
{
  if (m_shader && m_attribColor && RefreshOutputTexture(device))
  {
    device->SetRenderTarget(m_outputTarget);
    device->Clear(true, ce::Color4f(0, 0, 0, 0), false, 1.0f, false, 0);
    
    device->SetShader(m_shader);
    device->ResetTextures();
    ce::eTextureUnit unit = device->BindTexture(m_inputTexture);
    m_attribColor->Bind(unit);
    device->RenderFullscreen();
  }
}
