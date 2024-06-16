#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>

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
  T* Add()
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
    m_nodes.push_back(node);
    return t;
  }

  void CalcIOTypes() override;

  size_t GetNumberOfNodes() const;
  SGNode* GetNode(size_t idx);
  const SGNode* GetNode(size_t idx) const;

private:
  std::vector<SGNode*> m_nodes;


  SGNodeInput* m_diffuse;
  


};


}


