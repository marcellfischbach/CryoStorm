
#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iDevice.hh>

namespace cs
{


struct PPPlanProcess
{
  std::vector<csPPBind> Bindings;
  iPostProcess          *Process;
};

struct PPPlan
{
  std::vector<PPPlanProcess> Processes;
};


csPostProcessing::csPostProcessing()
    : iObject()
    , m_plan(new PPPlan)
{

  for (size_t i = 0; i < (size_t) ePPImageType::Count; i++)
  {
    m_inputTextures[i]  = nullptr;
    m_outputTextures[i] = nullptr;
  }
}

csPostProcessing::~csPostProcessing()
{
  for (auto process: m_processes)
  {
    CS_RELEASE(process);
  }
  m_processes.clear();
  m_bindings.clear();
}


void csPostProcessing::AddProcess(iPostProcess *process)
{
  CS_ADDREF(process);
  m_processes.push_back(process);
  m_planDirty = true;
}

void csPostProcessing::Bind(const cs::csPPBind &bind)
{
  if (bind.DstPP)
  {
    m_bindings.push_back(bind);
  }
  else
  {
    m_outputBindings.push_back(bind);
  }
  m_planDirty = true;
}

void csPostProcessing::SetInput(ePPImageType type, iTexture2D *texture)
{
  m_inputTextures[(size_t) type] = texture;
}

iTexture2D *csPostProcessing::GetOutput(ePPImageType type)
{
  return m_outputTextures[(size_t) type];
}


void csPostProcessing::Process(iDevice *device, iRenderTarget2D *finalTarget)
{
  if (m_planDirty)
  {
    RebuildPlan();
    m_planDirty = false;
  }

  for (const auto &process: m_plan->Processes)
  {
    for (const auto &binding: process.Bindings)
    {
      if (binding.SrcPP)
      {
        process.Process->SetInput(binding.DstIdx, binding.SrcPP->GetOutput(binding.SrcIdx));
      }
      else
      {
        process.Process->SetInput(binding.DstIdx, m_inputTextures[binding.SrcIdx]);
      }
    }

    process.Process->Process(device, process.Process == m_finalProcess ?  finalTarget : nullptr);
  }

  for (const auto &binding: m_outputBindings)
  {
    if (binding.SrcPP)
    {
      m_outputTextures[binding.DstIdx] = binding.SrcPP->GetOutput(binding.SrcIdx);
    }
    else
    {
      m_outputTextures[binding.DstIdx] = m_inputTextures[binding.SrcIdx];
    }
  }

}

void csPostProcessing::RebuildPlan()
{
  std::vector<iPostProcess *> pendingProcesses(m_processes);
  m_plan->Processes.clear();
  std::vector<iPostProcess *> processed;


  while (true)
  {
    bool      foundOne = false;
    for (auto it       = pendingProcesses.begin(); it != pendingProcesses.end(); it++)
    {
      iPostProcess          *pending         = *it;
      std::vector<csPPBind> bindings         = GetBindingsForProcess(pending);
      bool                  allBindingsFound = true;
      for (const auto     &binding: bindings)
      {
        if (binding.SrcPP && std::find(processed.begin(), processed.end(), binding.SrcPP) == processed.end())
        {
          //not processed yet
          allBindingsFound = false;
        }
      }

      if (allBindingsFound)
      {
        m_plan->Processes.push_back({bindings, pending});

        processed.push_back(pending);

        pendingProcesses.erase(it);
        foundOne = true;
        break;
      }
    }

    if (!foundOne)
    {
      break;
    }
  }

  if (!m_outputBindings.empty())
  {
    m_finalProcess = m_outputBindings[0].SrcPP;
  }
}


std::vector<csPPBind> csPostProcessing::GetBindingsForProcess(const iPostProcess *process)
{
  std::vector<csPPBind> result;
  for (const auto       &binding: m_bindings)
  {
    if (binding.DstPP == process)
    {
      result.push_back(binding);
    }
  }
  return result;
}


void csBasePostProcess::SetInput(size_t idx, cs::iTexture2D *texture)
{
  if (idx >= m_inputs.size())
  {
    return;
  }
  m_inputs[idx] = texture;
}

iTexture2D *csBasePostProcess::GetOutput(size_t idx) const
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }

  return m_outputs[idx];
}


csBasePostProcess::~csBasePostProcess()
{
  for (auto &input: m_inputs)
  {
    input = nullptr;
  }
  for (auto &output: m_outputs)
  {
    output = nullptr;
  }
}

const std::vector<csPPInputDefinition> &csBasePostProcess::GetInputDefinitions() const
{
  return m_inputDefinitions;
}

const std::vector<csPPOutputDefinition> &csBasePostProcess::GetOutputDefinitions() const
{
  return m_outputDefinitions;
}


void csBasePostProcess::DeclareInput(cs::ePPImageType type, const std::string &name)
{
  m_inputs.push_back(nullptr);
  m_inputDefinitions.push_back({m_inputs.size(), type, name});
}

void csBasePostProcess::DeclareOutput(cs::ePPImageType type, const std::string &name)
{
  m_outputs.push_back(nullptr);
  m_outputDefinitions.push_back({m_outputs.size(), type, name});
}


csSimplePostProcess::~csSimplePostProcess()
{
  CS_RELEASE(m_renderTarget);
}

bool csSimplePostProcess::UpdateRenderTarget(iDevice *device,
                                             uint32_t width,
                                             uint32_t height,
                                             bool color,
                                             size_t colorIdx,
                                             cs::ePixelFormat colorFormat,
                                             bool depth,
                                             size_t depthIdx,
                                             cs::ePixelFormat depthFormat)
{
  if (m_renderTarget != nullptr)
  {
    iTexture2D *colorTexture = m_renderTarget->GetColorTexture(0);
    iTexture2D *depthTexture = m_renderTarget->GetDepthTexture();
    if (m_renderTarget->GetWidth() == width
        && m_renderTarget->GetHeight() == height
        && (!color && !colorTexture || colorTexture && colorTexture->GetFormat() == colorFormat)
        && (!depth && !depthTexture || depthTexture && depthTexture->GetFormat() == depthFormat)
        )
    {
      return true;
    }
  }

  // clear the outputs
  CS_RELEASE(m_renderTarget);
  for (int i = 0 , in = m_outputs.size(); i<in; ++i)
  {
    m_outputs[i] = nullptr;
  }


  cs::iRenderTarget2D::Descriptor desc {(uint16_t) width, (uint16_t) height};
  m_renderTarget = device->CreateRenderTarget(desc);
  if (!m_renderTarget)
  {
    return false;
  }

  if (color)
  {
    cs::iTexture2D::Descriptor colorDesc {
        colorFormat,
        (uint16_t) width,
        (uint16_t) height,
        false,
        1
    };
    csRef<cs::iTexture2D> colorTexture = device->CreateTexture(colorDesc);

    csOwned<iSampler> oSampler = device->CreateSampler();
    iSampler *sampler = oSampler.Data();
    sampler->SetAddressU(eTAM_Clamp);
    sampler->SetAddressV(eTAM_Clamp);
    sampler->SetAddressW(eTAM_Clamp);
    sampler->SetFilterMode(eFM_MinMagNearest);
    colorTexture->SetSampler(sampler);

    m_renderTarget->AddColorTexture(colorTexture);

    if (colorIdx < m_outputs.size())
    {
      m_outputs[colorIdx]= colorTexture;
    }
  }

  if (depth)
  {
    cs::iTexture2D::Descriptor depthDesc {
        depthFormat,
        (uint16_t) width,
        (uint16_t) height,
        false,
        1
    };
    csRef<cs::iTexture2D> depthTexture = device->CreateTexture(depthDesc);


    m_renderTarget->SetDepthTexture(depthTexture);

    if (depthIdx < m_outputs.size())
    {
      m_outputs[depthIdx] = depthTexture;
    }
  }

  if (!m_renderTarget->Compile())
  {
    CS_RELEASE(m_renderTarget);
    for (auto &output: m_outputs)
    {
      output = nullptr;
    }
    return false;
  }


  return true;
}


}