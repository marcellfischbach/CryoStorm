
#pragma once

#include <ceCore/graphics/postprocessing.hh>

namespace ce
{



PostProcessing::PostProcessing()
  : iObject()
{
  CE_CLASS_GEN_CONSTR;
  m_textures[0] = nullptr;
  m_textures[1] = nullptr;
}

PostProcessing::~PostProcessing()
{
  for (auto process : m_processes)
  {
    CE_RELEASE(process);
  }
  m_processes.clear();
}


void PostProcessing::AddProcess(iPostProcess* process)
{
  CE_ADDREF(process);
  m_processes.push_back(process);
}


void PostProcessing::SetInput(PPImageType type, iTexture2D* texture)
{
  m_textures[(size_t)type] = texture;
}

iTexture2D *PostProcessing::GetOutput(PPImageType type) 
{
  return m_textures[(size_t)type];
}


void PostProcessing::Process(iDevice* device)
{
  for (auto process : m_processes)
  {
    for (auto input : process->GetInputDefinitions())
    {
      process->SetInput(input.Idx, m_textures[(size_t)input.Type]);
    }
    process->Process(device);
    for (auto output : process->GetOutputDefinitions())
    {
      m_textures[(size_t)output.Type] = process->GetOutput(output.Idx);
    }
  }
}

}