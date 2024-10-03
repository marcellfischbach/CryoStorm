
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/epixelformat.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <vector>


namespace cryo
{

struct iDevice;
struct iTexture2D;
struct iRenderTarget2D;

struct iPostProcess;

enum class PPImageType
{
  Color,
  Normal,
  Depth,
  Count
};

struct PPInputDefinition
{
  size_t      Idx;
  PPImageType Type;
  std::string Name;
};


struct PPOutputDefinition
{
  size_t      Idx;
  PPImageType Type;
  std::string Name;
};





CS_CLASS()
struct CS_CORE_API iPostProcess : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iPostProcess() override = default;

  virtual const std::vector<PPInputDefinition> &GetInputDefinitions() const = 0;
  virtual const std::vector<PPOutputDefinition> &GetOutputDefinitions() const = 0;

  virtual void SetInput(size_t idx, iTexture2D *texture) = 0;
  virtual iTexture2D *GetOutput(size_t idx) const = 0;

  virtual void Process(iDevice *device, iRenderTarget2D *finalTarget) = 0;

};


CS_CLASS()
class CS_CORE_API BasePostProcess : public CS_SUPER(iPostProcess)
{
CS_CLASS_GEN_OBJECT;
public:
  ~BasePostProcess() override;

  const std::vector<PPInputDefinition> &GetInputDefinitions() const override;
  const std::vector<PPOutputDefinition> &GetOutputDefinitions() const override;

  void SetInput(size_t idx, iTexture2D *texture) override;
  iTexture2D *GetOutput(size_t idx) const override;

protected:
  BasePostProcess() = default;

  void DeclareInput(PPImageType type, const std::string &name);
  void DeclareOutput(PPImageType type, const std::string &name);

private:
  std::vector<PPInputDefinition>  m_inputDefinitions;
  std::vector<PPOutputDefinition> m_outputDefinitions;

protected:
  std::vector<iTexture2D *> m_inputs;
  std::vector<iTexture2D *> m_outputs;
};

CS_CLASS()
class CS_CORE_API SimplePostProcess : public CS_SUPER(BasePostProcess)
{
CS_CLASS_GEN;
public:
  ~SimplePostProcess() override;

protected:
  SimplePostProcess() = default;

  bool UpdateRenderTarget(iDevice *device,
                          uint32_t width,
                          uint32_t height,
                          bool color,
                          size_t colorIdx,
                          ePixelFormat colorFormat,
                          bool depth,
                          size_t depthIdx,
                          ePixelFormat depthFormat);

protected:
  iRenderTarget2D *m_renderTarget = nullptr;
};


struct PPBind
{
  iPostProcess *SrcPP;
  size_t       SrcIdx;
  iPostProcess *DstPP;
  size_t       DstIdx;

};


struct PPPlan;

CS_CLASS()
class CS_CORE_API PostProcessing : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  PostProcessing();
  ~PostProcessing() override;

  void AddProcess(iPostProcess *process);
  void Bind(const PPBind &bind);

  void SetInput(PPImageType type, iTexture2D *texture);
  void Process(iDevice *device, iRenderTarget2D *finalTarget);
  iTexture2D *GetOutput(PPImageType type);

private:
  void RebuildPlan();
  std::vector<PPBind> GetBindingsForProcess(const iPostProcess *process);

public:
  iTexture2D *m_inputTextures[(size_t) PPImageType::Count];
  iTexture2D *m_outputTextures[(size_t) PPImageType::Count];

  std::vector<iPostProcess *> m_processes;
  iPostProcess                *m_finalProcess;
  std::vector<PPBind>         m_bindings;
  std::vector<PPBind>         m_outputBindings;

  bool m_planDirty = true;

  PPPlan *m_plan = nullptr;
};


}