//
// Created by Marcell on 21.10.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/iModule.hh>
namespace cs
{

CS_CLASS(jclass="org.cryo.core.CsJavaModule")
class CS_CORE_API csJavaModule : public CS_SUPER(iModule)
{
  CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, csEngine *engine) override;
  void Shutdown (const std::vector<std::string> &args, csEngine *engine) override;

};

} // cs
