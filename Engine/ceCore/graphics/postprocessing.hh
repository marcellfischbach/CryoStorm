
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
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

enum class PPInputSource
{
  Origin,
  LastStep,
};

struct PPInputDefinition
{
  size_t Idx;
  PPInputSource Source;
  PPImageType Type;
};


struct PPOutputDefinition
{
  size_t Idx;
  PPImageType Type;
};





CE_CLASS()
struct CE_CORE_API iPostProcess : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  ~iPostProcess() override = default;

  virtual const std::vector<PPInputDefinition>& GetInputDefinitions() const = 0;
  virtual const std::vector<PPOutputDefinition>& GetOutputDefinitions() const = 0;

  virtual void SetInput(size_t idx, iTexture2D * texture) = 0;
  virtual iTexture2D* GetOutput(size_t idx) const = 0;

  virtual void Process(iDevice * device) = 0;

};


CE_CLASS()
class CE_CORE_API BasePostProcess : public CE_SUPER(iPostProcess)
{
  CE_CLASS_GEN_OBJECT;
public:

  void SetInput(size_t idx, iTexture2D * texture) override;
  iTexture2D* GetOutput(size_t idx) const override;

protected:
  void DeclareInput ()
  void SetOutput(PPImageType type, iTexture2D* texture);

  std::vector<iTexture2D*> m_inputs;
  std::vector<iTexture2D*> m_outputs;
};


CE_CLASS()
class CE_CORE_API PostProcessing : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  PostProcessing();
  ~PostProcessing() override;

  void AddProcess(iPostProcess * process);

  void SetInput(PPImageType type, iTexture2D * texture);
  void Process(iDevice * device);
  iTexture2D* GetOutput(PPImageType type);


public:
  iTexture2D* m_textures[(size_t)PPImageType::Count];
  iTexture2D* m_originTextures[(size_t)PPImageType::Count];

  std::vector<iPostProcess*> m_processes;

};


}