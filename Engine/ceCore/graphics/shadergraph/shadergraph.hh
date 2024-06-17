#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>
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
  void BindDiffuse(SGNode * node, size_t outputIdx = 0);

  SGNodeInput* GetDiffuseInput();

  void CalcIOTypes() override;

  size_t GetNumberOfNodes() const;
  SGNode* GetNode(size_t idx);
  const SGNode* GetNode(size_t idx) const;

  void SetAlphaDiscard(float threshold, eCompareFunc compreFunc);
  float GetAlphaDiscard_Threshold() const;
  eCompareFunc GetAlphaDiscard_Func() const;

private:
  std::vector<SGNode*> m_nodes;


  SGNodeInput* m_diffuse = nullptr;
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



}


