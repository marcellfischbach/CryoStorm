
#include <csCore/graphics/csGBuffer.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/iDevice.hh>


namespace cs
{

csGBuffer::csGBuffer()
    : m_gBuffer(nullptr)
    , m_diffuseRoughness(nullptr)
    , m_depth(nullptr)
    , m_normal(nullptr)
    , m_emissionMetallic(nullptr)
    , m_diffuseRoughnessSampler(nullptr)
    , m_depthSampler(nullptr)
    , m_normalSampler(nullptr)
    , m_emissionMetallicSampler(nullptr)
{

}


bool csGBuffer::Update(cs::iDevice *device, uint16_t width, uint16_t height)
{
  if (m_diffuseRoughness)
  {
    if (m_diffuseRoughness->GetWidth() == width && m_diffuseRoughness->GetHeight() == height)
    {
      return true;
    }
    CS_RELEASE(m_gBuffer);
    CS_RELEASE(m_diffuseRoughness);
    CS_RELEASE(m_depth);
    CS_RELEASE(m_normal);
    CS_RELEASE(m_emissionMetallic);
  }

  device->ClearTextureCache();
  UpdateSamplers(device);

  m_bufferIds.clear();
  uint32_t bufferId = 0;
  {
    iTexture2D::Descriptor desc{ePixelFormat::ePF_RGBA, width, height, false, 1};
    m_diffuseRoughness = device->CreateTexture(desc);
    m_diffuseRoughness->SetSampler(m_diffuseRoughnessSampler);
    m_bufferIds.push_back(bufferId++);
  }
  {
    iTexture2D::Descriptor desc{ePixelFormat::ePF_Depth, width, height, false, 1};
    m_depth = device->CreateTexture(desc);
    m_depth->SetSampler(m_depthSampler);
  }
  {
    iTexture2D::Descriptor desc{ePixelFormat::ePF_RGBA, width, height, false, 1};
    m_normal = device->CreateTexture(desc);
    m_normal->SetSampler(m_normalSampler);
    m_bufferIds.push_back(bufferId++);
  }
  {
    iTexture2D::Descriptor desc{ePixelFormat::ePF_RGBA, width, height, false, 1};
    m_emissionMetallic = device->CreateTexture(desc);
    m_emissionMetallic->SetSampler(m_emissionMetallicSampler);
    m_bufferIds.push_back(bufferId++);
  }

  {
    iRenderTarget2D::Descriptor desc { width, height };
    m_gBuffer = device->CreateRenderTarget(desc);
    m_gBuffer->AddColorTexture(m_diffuseRoughness);
    m_gBuffer->AddColorTexture(m_normal);
    m_gBuffer->AddColorTexture(m_emissionMetallic);
    m_gBuffer->SetDepthTexture(m_depth);
    if (!m_gBuffer->Compile())
    {
      std::string log = m_gBuffer->GetCompileLog();
      printf ("Unable to compile GBuffer: %s\n", log.c_str());
      return false;
    }
  }
  device->ClearTextureCache();
  return true;
}


void csGBuffer::UpdateSamplers(cs::iDevice *device)
{
  if (m_diffuseRoughnessSampler != nullptr)
  {
    return;
  }

  m_diffuseRoughnessSampler = device->CreateSampler();
  m_diffuseRoughnessSampler->SetFilterMode(eFilterMode::eFM_MinMagNearest);

  m_depthSampler = device->CreateSampler();
  m_depthSampler->SetFilterMode(eFilterMode::eFM_MinMagNearest);
  m_depthSampler->SetTextureCompareMode(eTextureCompareMode::eTCM_None);
  m_depthSampler->SetTextureCompareFunc(eCompareFunc::eCF_Always);

  m_normalSampler = device->CreateSampler();
  m_normalSampler->SetFilterMode(eFilterMode::eFM_MinMagNearest);

  m_emissionMetallicSampler = device->CreateSampler();
  m_emissionMetallicSampler->SetFilterMode(eFilterMode::eFM_MinMagNearest);
}

iTexture2D *csGBuffer::GetDiffuseRoughness() const
{
  return m_diffuseRoughness;
}

iTexture2D *csGBuffer::GetDepth() const
{
  return m_depth;
}

iTexture2D *csGBuffer::GetNormal() const
{
  return m_normal;
}

iTexture2D *csGBuffer::GetEmissionMetallic() const
{
  return m_emissionMetallic;
}

iRenderTarget2D *csGBuffer::GetGBuffer() const
{
  return m_gBuffer;
}

const std::vector<uint32_t> & csGBuffer::GetBufferIDs() const
{
  return m_bufferIds;
}


}