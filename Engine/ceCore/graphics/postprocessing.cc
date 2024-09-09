
#include <ceCore/graphics/postprocessing.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/idevice.hh>

namespace ce
{


struct PPPlanProcess
{
  std::vector<PPBind> Bindings;
  iPostProcess        *Process;
};

struct PPPlan
{
  std::vector<PPPlanProcess> Processes;
};


PostProcessing::PostProcessing()
    : iObject()
    , m_plan(new PPPlan)
{
  CE_CLASS_GEN_CONSTR;
  for (size_t i = 0; i < (size_t) PPImageType::Count; i++)
  {
    m_inputTextures[i]  = nullptr;
    m_outputTextures[i] = nullptr;
  }
}

PostProcessing::~PostProcessing()
{
  for (auto process: m_processes)
  {
    CE_RELEASE(process);
  }
  m_processes.clear();
  m_bindings.clear();
}


void PostProcessing::AddProcess(iPostProcess *process)
{
  CE_ADDREF(process);
  m_processes.push_back(process);
  m_planDirty = true;
}

void PostProcessing::Bind(const ce::PPBind &bind)
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

void PostProcessing::SetInput(PPImageType type, iTexture2D *texture)
{
  m_inputTextures[(size_t) type] = texture;
}

iTexture2D *PostProcessing::GetOutput(PPImageType type)
{
  return m_outputTextures[(size_t) type];
}


void PostProcessing::Process(iDevice *device, iRenderTarget2D *finalTarget)
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

void PostProcessing::RebuildPlan()
{
  std::vector<iPostProcess *> pendingProcesses(m_processes);
  m_plan->Processes.clear();
  std::vector<iPostProcess *> processed;


  while (true)
  {
    bool      foundOne = false;
    for (auto it       = pendingProcesses.begin(); it != pendingProcesses.end(); it++)
    {
      iPostProcess        *pending         = *it;
      std::vector<PPBind> bindings         = GetBindingsForProcess(pending);
      bool                allBindingsFound = true;
      for (const auto     &binding: bindings)
      {
        if (binding.SrcPP && std::ranges::find(processed.begin(), processed.end(), binding.SrcPP) == processed.end())
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


std::vector<PPBind> PostProcessing::GetBindingsForProcess(const iPostProcess *process)
{
  std::vector<PPBind> result;
  for (const auto     &binding: m_bindings)
  {
    if (binding.DstPP == process)
    {
      result.push_back(binding);
    }
  }
  return result;
}


void BasePostProcess::SetInput(size_t idx, ce::iTexture2D *texture)
{
  if (idx >= m_inputs.size())
  {
    return;
  }
  CE_SET(m_inputs[idx], texture);
}

iTexture2D *BasePostProcess::GetOutput(size_t idx) const
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }

  return m_outputs[idx];
}


BasePostProcess::~BasePostProcess()
{
  for (auto &input: m_inputs)
  {
    CE_RELEASE(input);
  }
  for (auto &output: m_outputs)
  {
    CE_RELEASE(output);
  }
}

const std::vector<PPInputDefinition> &BasePostProcess::GetInputDefinitions() const
{
  return m_inputDefinitions;
}

const std::vector<PPOutputDefinition> &BasePostProcess::GetOutputDefinitions() const
{
  return m_outputDefinitions;
}


void BasePostProcess::DeclareInput(ce::PPImageType type, const std::string &name)
{
  m_inputs.push_back(nullptr);
  m_inputDefinitions.push_back({m_inputs.size(), type, name});
}

void BasePostProcess::DeclareOutput(ce::PPImageType type, const std::string &name)
{
  m_outputs.push_back(nullptr);
  m_outputDefinitions.push_back({m_outputs.size(), type, name});
}


SimplePostProcess::~SimplePostProcess()
{
  CE_RELEASE(m_renderTarget);
}

bool SimplePostProcess::UpdateRenderTarget(iDevice *device,
                                           uint32_t width,
                                           uint32_t height,
                                           bool color,
                                           size_t colorIdx,
                                           ce::ePixelFormat colorFormat,
                                           bool depth,
                                           size_t depthIdx,
                                           ce::ePixelFormat depthFormat)
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
  CE_RELEASE(m_renderTarget);
  for (int i = 0 , in = m_outputs.size(); i<in; ++i)
  {
    CE_RELEASE(m_outputs[i]);
    m_outputs[i] = nullptr;
  }


  ce::iRenderTarget2D::Descriptor desc {(uint16_t) width, (uint16_t) height};
  m_renderTarget = device->CreateRenderTarget(desc);
  if (!m_renderTarget)
  {
    return false;
  }

  if (color)
  {
    ce::iTexture2D::Descriptor colorDesc {
        colorFormat,
        (uint16_t) width,
        (uint16_t) height,
        false,
        1
    };
    ce::iTexture2D             *colorTexture = device->CreateTexture(colorDesc);

    iSampler *sampler = device->CreateSampler();
    sampler->SetAddressU(eTAM_Clamp);
    sampler->SetAddressV(eTAM_Clamp);
    sampler->SetAddressW(eTAM_Clamp);
    sampler->SetFilterMode(eFM_MinMagNearest);
    colorTexture->SetSampler(sampler);

    m_renderTarget->AddColorTexture(colorTexture);

    if (colorIdx < m_outputs.size())
    {
      CE_SET(m_outputs[colorIdx], colorTexture);
    }
    CE_RELEASE(colorTexture);
  }

  if (depth)
  {
    ce::iTexture2D::Descriptor depthDesc {
        depthFormat,
        (uint16_t) width,
        (uint16_t) height,
        false,
        1
    };
    ce::iTexture2D             *depthTexture = device->CreateTexture(depthDesc);


    m_renderTarget->SetDepthTexture(depthTexture);

    if (depthIdx < m_outputs.size())
    {
      CE_SET(m_outputs[depthIdx], depthTexture);
    }
    CE_RELEASE(depthTexture);
  }

  if (!m_renderTarget->Compile())
  {
    CE_RELEASE(m_renderTarget);
    for (const auto &output: m_outputs)
    {
      CE_RELEASE(output);
    }
    return false;
  }


  return true;
}


}