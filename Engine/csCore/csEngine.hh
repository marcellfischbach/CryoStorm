//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csFPS.hh>
#include <csCore/csRef.hh>
#include <vector>
#include "iGame.hh"

namespace cs
{

struct iDevice;
struct iModule;
struct iFrameRenderer;
struct iRenderTarget2D;
struct iSkyboxRenderer;
struct iWindow;

class csWorld;

CS_CLASS(jclass="org.cryo.core.ModuleConfig")
class CS_CORE_API csModuleConfig : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csModuleConfig();
  ~csModuleConfig() override = default;

  CS_FUNCTION()
  bool LoadModuleConfig();

  CS_FUNCTION()
  bool LoadModuleConfigEx(const std::string &configFilename);

  CS_FUNCTION()
  void AddModule(cs::iModule *module);

  CS_FUNCTION()
  void AddModuleByName(const std::string &moduleName);

  const std::vector<csRef<iModule>> &GetModules() const;

private:
  std::vector<csRef<iModule>> m_modules;

};


CS_CLASS(jclass="org.cryo.core.Engine")
class CS_CORE_API csEngine : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:

  CS_FUNCTION()
  bool InitializeEngine(const std::vector<std::string> &args, const cs::csModuleConfig &moduleConfig);

  CS_FUNCTION()
  void ShutdownEngine(const std::vector<std::string> &args, const cs::csModuleConfig &moduleConfig);

  CS_NODISCARD bool ShouldExit() const;
  void Exit(int returnValue = 0);
  CS_NODISCARD int ExitValue() const;

private:

  bool shouldExit = false;
  int m_exitValue = 0;
public:
  static csEngine *Get();

private:
  csEngine();

  static csEngine *s_instance;
};

} // ce
