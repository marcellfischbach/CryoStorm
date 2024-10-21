//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csFPS.hh>
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

CS_CLASS()
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
  void AddModule(iModule *module);

  CS_FUNCTION()
  void AddModuleByName(const std::string &moduleName);

  const std::vector<iModule *> &GetModules() const;

private:
  std::vector<iModule *> m_modules;

};

class CS_CORE_API  csEngine
{
public:
  bool InitializeEngine(const std::vector<std::string> &args, const csModuleConfig &moduleConfig);

  CS_NODISCARD bool ShouldExit() const;
  void Exit(int returnValue = 0);
  CS_NODISCARD int ExitValue() const;

private:

  bool shouldExit = false;
  int m_exitValue = 0;
public:
  static csEngine *Get();

private:
  csEngine() = default;

  static csEngine *s_instance;
};

} // ce
