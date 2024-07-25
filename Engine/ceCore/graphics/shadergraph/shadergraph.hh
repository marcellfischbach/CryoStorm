#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

#include <ceCore/graphics/eblendfactor.hh>
#include <ceCore/graphics/ecomparefunc.hh>
#include <ceCore/graphics/erenderqueue.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>
#include <ceCore/graphics/itexture.hh>
#include <array>

namespace ce
{


CE_CLASS()
class CE_CORE_API ShaderGraph : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  ShaderGraph();
  ~ShaderGraph() override;

  template<typename T>
  T *Add(const std::string &key);
  SGNode *Add(const Class *nodeClass, const std::string &key);

  template<typename T>
  T *AddResource(const std::string &key, const std::string &resourceName);
  SGResourceNode *AddResource(const Class *nodeClass, const std::string &key, const std::string &resourceName);

  void BindDiffuse(SGNode *node, size_t outputIdx = 0);
  void BindAlpha(SGNode *node, size_t outputIdx = 0);
  void BindRoughness(SGNode *node, size_t outputIdx = 0);
  void BindNormal(SGNode *node, size_t outputIdx = 0);
  void BindMetallic(SGNode *node, size_t outputIdx = 0);

  SGNodeInput *GetDiffuseInput();
  SGNodeInput *GetAlphaInput();
  SGNodeInput *GetRoughnessInput();
  SGNodeInput *GetNormalInput();
  SGNodeInput *GetMetallicInput();

  void CalcIOTypes() override;

  size_t GetNumberOfNodes() const;
  SGNode *GetNode(size_t idx);
  const SGNode *GetNode(size_t idx) const;
  SGNode *GetNode(const std::string &key);
  const SGNode *GetNode(const std::string &key) const;

  void SetReceiveShadow(bool receiveShadow);
  bool IsReceiveShadow() const;

  void SetAlphaDiscard(float threshold, eCompareFunc compreFunc);
  float GetAlphaDiscard_Threshold() const;
  eCompareFunc GetAlphaDiscard_Func() const;

  void SetDefault(const std::string &attribute, size_t count, float *floats);
  void SetDefault(const std::string &attribute, size_t count, int *ints);
  void SetDefault(const std::string &attribute, iTexture *texture);

  struct Default
  {
    std::string           name;
    std::array<float, 16> floats;
    std::array<int, 4>    ints;
    iTexture              *texture;
  };

  const Default *GetDefault(const std::string &name) const;

  enum eLightingMode
  {
    eLM_Default,
    eLM_Blend,
    eLM_Add,
  };

  void SetLightingMode (eLightingMode lightingMode);
  eLightingMode GetLightingMode () const;


  void SetQueue(eRenderQueue queue);
  eRenderQueue GetQueue() const;


private:


  std::vector<Default>  m_defaults;
  std::vector<SGNode *> m_nodes;

  eRenderQueue m_queue          = eRenderQueue::Default;
  eLightingMode m_lightingMode = eLM_Default;


  SGNodeInput *m_diffuse   = nullptr;
  SGNodeInput *m_alpha     = nullptr;
  SGNodeInput *m_roughness = nullptr;
  SGNodeInput *m_normal    = nullptr;
  SGNodeInput *m_metallic  = nullptr;

  bool         m_receiveShadow          = true;
  float        m_alphaDiscard_Threshold = 0.5f;
  eCompareFunc m_alphaDiscard_Func      = eCF_Never;


};

template<typename T>
T *ShaderGraph::Add(const std::string &key)
{
  const Class *nodeClass = T::GetStaticClass();

  SGNode *node = Add(nodeClass, key);

  return static_cast<T *>(node);

}


template<typename T>
T *ShaderGraph::AddResource(const std::string &key, const std::string &resourceName)
{
  const Class *nodeClass = T::GetStaticClass();

  SGNode *node = AddResource(nodeClass, key, resourceName);

  return static_cast<T *>(node);
}


}


