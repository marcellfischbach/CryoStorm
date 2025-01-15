
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/graphics/ePixelFormat.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/csRef.hh>
#include <vector>


namespace cs
{

struct iDevice;
struct iTexture2D;
struct iRenderTarget2D;

struct iPostProcess;

enum class ePPImageType
{
  Color,
  Normal,
  Depth,
  Count
};

struct csPPInputDefinition
{
  size_t       Idx;
  ePPImageType Type;
  std::string  Name;
};


struct csPPOutputDefinition
{
  size_t       Idx;
  ePPImageType Type;
  std::string  Name;
};





CS_CLASS()
struct CS_CORE_API iPostProcess : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iPostProcess() override = default;

  virtual const std::vector<csPPInputDefinition> &GetInputDefinitions() const = 0;
  virtual const std::vector<csPPOutputDefinition> &GetOutputDefinitions() const = 0;

  virtual void SetInput(size_t idx, iTexture2D *texture) = 0;
  virtual iTexture2D *GetOutput(size_t idx) const = 0;

  virtual void Process(iDevice *device, iRenderTarget2D *finalTarget) = 0;

};


CS_CLASS()
class CS_CORE_API csBasePostProcess : public CS_SUPER(iPostProcess)
{
CS_CLASS_GEN_OBJECT;
public:
  ~csBasePostProcess() override;

  const std::vector<csPPInputDefinition> &GetInputDefinitions() const override;
  const std::vector<csPPOutputDefinition> &GetOutputDefinitions() const override;

  void SetInput(size_t idx, iTexture2D *texture) override;
  iTexture2D *GetOutput(size_t idx) const override;

protected:
  csBasePostProcess() = default;

  void DeclareInput(ePPImageType type, const std::string &name);
  void DeclareOutput(ePPImageType type, const std::string &name);

private:
  std::vector<csPPInputDefinition>  m_inputDefinitions;
  std::vector<csPPOutputDefinition> m_outputDefinitions;

protected:
  std::vector<csAssetRef<iTexture2D>> m_inputs;
  std::vector<csAssetRef<iTexture2D>> m_outputs;
};

CS_CLASS()
class CS_CORE_API csSimplePostProcess : public CS_SUPER(csBasePostProcess)
{
CS_CLASS_GEN;
public:
  ~csSimplePostProcess() override;

protected:
  csSimplePostProcess() = default;

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
  csRef<iRenderTarget2D> m_renderTarget = nullptr;
};


struct csPPBind
{
  iPostProcess *SrcPP;
  size_t       SrcIdx;
  iPostProcess *DstPP;
  size_t       DstIdx;

};


struct PPPlan;

CS_CLASS()
class CS_CORE_API csPostProcessing : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csPostProcessing();
  ~csPostProcessing() override;

  void AddProcess(iPostProcess *process);
  void Bind(const csPPBind &bind);

  void SetInput(ePPImageType type, iTexture2D *texture);
  void Process(iDevice *device, iRenderTarget2D *finalTarget);
  iTexture2D *GetOutput(ePPImageType type);

private:
  void RebuildPlan();
  std::vector<csPPBind> GetBindingsForProcess(const iPostProcess *process);

public:
  csAssetRef<iTexture2D> m_inputTextures[(size_t) ePPImageType::Count];
  csAssetRef<iTexture2D> m_outputTextures[(size_t) ePPImageType::Count];

  std::vector<iPostProcess *> m_processes;
  iPostProcess          *m_finalProcess;
  std::vector<csPPBind> m_bindings;
  std::vector<csPPBind> m_outputBindings;

  bool m_planDirty = true;

  PPPlan *m_plan = nullptr;
};


}