
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


namespace ce
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





CE_CLASS()
struct CE_CORE_API iPostProcess : public CE_SUPER(iObject)
{
CE_CLASS_GEN;

  ~iPostProcess() override = default;

  virtual const std::vector<PPInputDefinition> &GetInputDefinitions() const = 0;
  virtual const std::vector<PPOutputDefinition> &GetOutputDefinitions() const = 0;

  virtual void SetInput(size_t idx, iTexture2D *texture) = 0;
  virtual iTexture2D *GetOutput(size_t idx) const = 0;

  virtual void Process(iDevice *device) = 0;

};


CE_CLASS()
class CE_CORE_API BasePostProcess : public CE_SUPER(iPostProcess)
{
CE_CLASS_GEN_OBJECT;
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
  std::vector<iTexture2D *>      m_inputs;
  std::vector<iTexture2D *>      m_outputs;
};

CE_CLASS()
class CE_CORE_API SimplePostProcess : public CE_SUPER(BasePostProcess)
{
CE_CLASS_GEN;
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
  iPostProcess *PP;
  size_t       Idx;
  iPostProcess *SrcPP;
  size_t       SrcIdx;
};


struct PPPlan;

CE_CLASS()
class CE_CORE_API PostProcessing : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;
public:
  PostProcessing();
  ~PostProcessing() override;

  void AddProcess(iPostProcess *process);
  void Bind(const PPBind &bind);
  void BindOutput(const PPBind &bind);

  void SetInput(PPImageType type, iTexture2D *texture);
  void Process(iDevice *device);
  iTexture2D *GetOutput(PPImageType type);

private:
  void RebuildPlan();
  std::vector<PPBind> GetBindingsForProcess(const iPostProcess *process);

public:
  iTexture2D *m_inputTextures[(size_t) PPImageType::Count];
  iTexture2D *m_outputTextures[(size_t) PPImageType::Count];

  std::vector<iPostProcess *> m_processes;
  std::vector<PPBind>         m_bindings;
  std::vector<PPBind>         m_outputBindings;

  bool m_planDirty = true;

  PPPlan *m_plan = nullptr;
};


}