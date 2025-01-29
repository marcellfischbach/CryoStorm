#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>

#include <csCore/graphics/eBlendFactor.hh>
#include <csCore/graphics/eCompareFunc.hh>
#include <csCore/graphics/eRenderQueue.hh>
#include <csCore/graphics/material/csMaterial.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <csCore/graphics/iTexture.hh>
#include <array>

namespace cs
{


CS_CLASS()
class CS_CORE_API csShaderGraph : public CS_SUPER(csMaterial)
{
CS_CLASS_GEN;
public:
  csShaderGraph() = default;
  ~csShaderGraph() override = default;

  csSGNode* Root();

  template<typename T>
  csOwned<T> Add(const std::string &key);
  csOwned<csSGNode> Add(const csClass *nodeClass, const std::string &key);

  template<typename T>
  csOwned<T> AddResource(const std::string &key, const std::string &resourceName);
  csOwned<csSGResourceNode>
  AddResource(const csClass *nodeClass, const std::string &key, const std::string &resourceName);

  bool Remove(csSGNode *node);

  void BindDiffuse(csSGNode *node, size_t outputIdx = 0);
  void BindAlpha(csSGNode *node, size_t outputIdx = 0);
  void BindRoughness(csSGNode *node, size_t outputIdx = 0);
  void BindNormal(csSGNode *node, size_t outputIdx = 0);
  void BindMetallic(csSGNode *node, size_t outputIdx = 0);

  csSGNodeInput *GetDiffuseInput() const;
  csSGNodeInput *GetAlphaInput() const;
  csSGNodeInput *GetRoughnessInput() const;
  csSGNodeInput *GetNormalInput() const;
  csSGNodeInput *GetMetallicInput() const;

  size_t GetNumberOfNodes() const;
  csSGNode *GetNode(size_t idx);
  const csSGNode *GetNode(size_t idx) const;
  csSGNode *GetNode(const std::string &key);
  const csSGNode *GetNode(const std::string &key) const;

  void SetReceiveShadow(bool receiveShadow);
  bool IsReceiveShadow() const;

  void SetAlphaDiscard(float threshold, eCompareFunc compreFunc);
  float GetAlphaDiscard_Threshold() const;
  eCompareFunc GetAlphaDiscard_Func() const;


  enum eBlendingMode
  {
    eBM_Off,
    eBM_Alpha,
    eBM_Add,
  };

  enum eLightingMode
  {
    eLM_Default,
    eLM_Attenuated,
    eLM_Unlit
  };

  void SetLightingMode(eLightingMode lightingMode);
  eLightingMode GetLightingMode() const;

  void SetBlendingMode(eBlendingMode blendingMode);
  eBlendingMode GetBlendingMode() const;


private:

  std::vector<csRef<csSGNode>> m_nodes;

  eLightingMode m_lightingMode = eLM_Default;
  eBlendingMode m_blendingMode = eBM_Off;




  bool m_receiveShadow = true;
  float m_alphaDiscard_Threshold = 0.5f;
  eCompareFunc m_alphaDiscard_Func = eCF_Never;


  class ShaderGraphNode  : public csSGNode
  {
  public:
    ShaderGraphNode();
    ~ShaderGraphNode() override = default;

    void CalcIOTypes() override;

    csRef<csSGNodeInput> m_diffuse = nullptr;
    csRef<csSGNodeInput> m_alpha = nullptr;
    csRef<csSGNodeInput> m_roughness = nullptr;
    csRef<csSGNodeInput> m_normal = nullptr;
    csRef<csSGNodeInput> m_metallic = nullptr;
  };

  csRef<ShaderGraphNode> m_node = csRef<ShaderGraphNode>(new ShaderGraphNode());


};

template<typename T>
csOwned<T> csShaderGraph::Add(const std::string &key)
{
  const csClass *nodeClass = T::GetStaticClass();

  csOwned<csSGNode> node = Add(nodeClass, key);

  return csOwned<T>(static_cast<T *>(node.Data()));

}


template<typename T>
csOwned<T> csShaderGraph::AddResource(const std::string &key, const std::string &resourceName)
{
  const csClass *nodeClass = T::GetStaticClass();

  csOwned<csSGNode> node = AddResource(nodeClass, key, resourceName);

  return csOwned<T>(static_cast<T *>(node.Data()));
}


}


