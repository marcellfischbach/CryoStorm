
#include <ceCore/graphics/skybox/simpleskybox.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{

void SimpleSkybox::Render(iDevice *device)
{
  if (!Prep(device))
  {
    return;
  }

  std::vector<uint32_t> buffers;
  buffers.push_back(0);
  buffers.push_back(1);
  buffers.push_back(2);
  buffers.push_back(3);
  buffers.push_back(4);
  buffers.push_back(5);

  device->SetRenderTarget(m_skyboxTarget);
  device->SetRenderBuffer(buffers);
  device->SetColorWrite(true, true, true, true);
  device->SetDepthWrite(false);
  device->SetDepthTest(false);
  device->Clear(true, Color4f(0.5f, 0.0f, 0.0f, 1.0f), false, 1.0f, false, 0);

  device->SetRenderBuffer(0);
}


bool SimpleSkybox::Prep(iDevice *device)
{
  return PrepShader() && PrepRenderTarget(device);
}

bool SimpleSkybox::PrepShader()
{
  if (m_prepShader)
  {
    return true;
  }

  m_prepShader = AssetManager::Get()->Get<iShader>("/shaders/gl4/skybox/simple/simple_skybox.shader");
  return m_prepShader;
}

bool SimpleSkybox::PrepRenderTarget(ce::iDevice *device)
{
  if (m_skyboxTexture && m_skyboxTarget)
  {
    return true;
  }

  CE_RELEASE(m_skyboxTarget);
  CE_RELEASE(m_skyboxTexture);

  iTextureCube::Descriptor txtDesc {
      ePF_RGB,
      1024,
      false // maybe later
  };

  m_skyboxTexture = device->CreateTexture(txtDesc);
  if (!m_skyboxTexture)
  {
    return false;
  }

  iRenderTargetCube::Descriptor trgtDesc {
    1024
  };
  m_skyboxTarget = device->CreateRenderTarget(trgtDesc);
  if (!m_skyboxTarget)
  {
    CE_RELEASE(m_skyboxTexture);
    m_skyboxTexture = nullptr;
    return false;
  }

  m_skyboxTarget->AddColorTexture(m_skyboxTexture);
  return m_skyboxTarget->Compile();
}


iTextureCube *SimpleSkybox::GetTexture() const
{
  return m_skyboxTexture;
}

} // ce