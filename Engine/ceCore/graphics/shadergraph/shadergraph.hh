#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>
#include <ceCore/graphics/ecomparefunc.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API ShaderGraph : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  ShaderGraph();
  ~ShaderGraph() override;

  template<typename T>T* Add(const std::string &info = std::string (""));
  template<typename T>T* AddResource(const std::string &resourceName, const std::string &info = std::string (""));
  void BindDiffuse(SGNode * node, size_t outputIdx = 0);
  void BindAlpha(SGNode * node, size_t outputIdx = 0);
  void BindRoughness(SGNode * node, size_t outputIdx = 0);
  void BindNormal(SGNode * node, size_t outputIdx = 0);
  void BindMetallic(SGNode * node, size_t outputIdx = 0);

  SGNodeInput* GetDiffuseInput();
  SGNodeInput* GetAlphaInput();
  SGNodeInput* GetRoughnessInput();
  SGNodeInput* GetNormalInput();
  SGNodeInput* GetMetallicInput();

  void CalcIOTypes() override;

  size_t GetNumberOfNodes() const;
  SGNode* GetNode(size_t idx);
  const SGNode* GetNode(size_t idx) const;

  void SetReceiveShadow (bool receiveShadow);
  bool IsReceiveShadow () const;


  void SetAlphaDiscard(float threshold, eCompareFunc compreFunc);
  float GetAlphaDiscard_Threshold() const;
  eCompareFunc GetAlphaDiscard_Func() const;

private:
  std::vector<SGNode*> m_nodes;


  SGNodeInput* m_diffuse = nullptr;
  SGNodeInput* m_alpha = nullptr;
  SGNodeInput* m_roughness = nullptr;
  SGNodeInput* m_normal = nullptr;
  SGNodeInput* m_metallic = nullptr;

  bool m_receiveShadow = true;
  float m_alphaDiscard_Threshold = 0.5f;
  eCompareFunc m_alphaDiscard_Func = eCF_Never;



};

template<typename T> 
T* ShaderGraph::Add(const std::string &info)
{
  if (!T::GetStaticClass()->IsInstanceOf<SGNode>())
  {
    return nullptr;
  }
  T* t = T::GetStaticClass()->CreateInstance<T>();
  if (!t)
  {
    return nullptr;
  }

  SGNode* node = t->Query<SGNode>();
  if (!node)
  {
    t->Release();
    return nullptr;
  }
  node->SetInfo(info);
  m_nodes.push_back(node);
  return t;
}



template<typename T>
T* ShaderGraph::AddResource(const std::string &resourceName, const std::string &info)
{

  if (!T::GetStaticClass()->IsInstanceOf<SGResourceNode>())
  {
    return nullptr;
  }
  T* t = T::GetStaticClass()->CreateInstance<T>();
  if (!t)
  {
    return nullptr;
  }

  SGResourceNode* node = t->Query<SGResourceNode>();
  if (!node)
  {
    t->Release();
    return nullptr;
  }
  node->SetResourceName(resourceName);
  node->SetInfo(info);
  m_nodes.push_back(node);
  return t;
}



}


