#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/shadergraph/esgvaluetype.hh>
#include <ceCore/math/vector.hh>
#include <utility>
#include <string>
#include <set>
#include <vector>

namespace ce
{

eSGValueType EvalValueType(eSGValueType v0, eSGValueType v1);

class SGNode;
class SGNodeInput;
class SGNodeOutput;

CE_CLASS()
class CE_CORE_API SGNodeIO : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;
public:
  CE_NODISCARD const SGNode *GetNode() const
  { return m_node; }
  CE_NODISCARD SGNode *GetNode()
  { return m_node; }

  CE_NODISCARD const std::string &GetName() const
  { return m_name; }

  void SetTypes(eSGValueType types)
  { m_types = types; }
  CE_NODISCARD eSGValueType GetTypes() const
  { return m_types; }

protected:
  SGNodeIO(SGNode *node, std::string name) : m_node(node), m_name(std::move(name))
  {}
  virtual ~SGNodeIO()
  { m_node = nullptr; }

private:

  SGNode *m_node;
  std::string m_name;
  eSGValueType m_types = eSGValueType::Invalid;

};

CE_CLASS()
class CE_CORE_API SGNodeOutput : public CE_SUPER(SGNodeIO)
{
  CE_CLASS_GEN;
public:
  SGNodeOutput(SGNode *node, const std::string &name) : SGNodeIO(node, name)
  {}
  ~SGNodeOutput() override
  { m_destinations.clear(); }


  void Add(SGNodeInput *input)
  { m_destinations.insert(input); }
  void Remove(SGNodeInput *input)
  { m_destinations.erase(input); }
  CE_NODISCARD const std::set<SGNodeInput *> &GetInputs() const
  { return m_destinations; }

  CE_NODISCARD eSGValueType GetValueType() const
  {
    return m_valueType;
  }
  void SetValueType(eSGValueType valueType)
  {
    m_valueType = valueType;
  }
protected:

private:
  eSGValueType            m_valueType = eSGValueType::Invalid;
  std::set<SGNodeInput *> m_destinations;
};

CE_CLASS()
class CE_CORE_API SGNodeInput : public CE_SUPER(SGNodeIO)
{
  CE_CLASS_GEN;
public:
  SGNodeInput(SGNode *node, const std::string &name) : SGNodeIO(node, name)
  {}
  ~SGNodeInput() override = default;

  void SetSource(SGNodeOutput *output)
  { m_source = output; }
  CE_NODISCARD SGNodeOutput *GetSource()
  { return m_source; }
  CE_NODISCARD const SGNodeOutput *GetSource() const
  { return m_source; }
  CE_NODISCARD float GetScalar() const
  {
    return m_scalar;
  }
  void SetScalar(float scalar)
  {
    m_scalar = scalar;
  }

  eSGValueType GetInputValueType() const;

private:
  SGNodeOutput *m_source = nullptr;

  float m_scalar;
};


CE_CLASS()
class CE_CORE_API SGNode : public CE_SUPER(iObject)
{
  CE_CLASS_GEN_OBJECT;

public:
  ~SGNode() override = default;

  void SetKey(const std::string &key);
  const std::string &GetKey() const;

  void SetInfo(const std::string &info);
  const std::string &GetInfo () const;

  const std::string& GetName() const;

  virtual void CalcIOTypes() = 0;

  void Bind(size_t inputIdx, SGNode* node, size_t outputIdx = 0);

  size_t GetNumberOfInputs() const;
  SGNodeInput* GetInput(size_t idx);
  const SGNodeInput* GetInput(size_t idx) const;

  size_t GetNumberOfOutputs() const;
  SGNodeOutput* GetOutput(size_t idx = 0);
  const SGNodeOutput* GetOutput(size_t idx = 0) const;

protected:
  explicit SGNode(const std::string &name);

  SGNodeInput* DefineInput(const std::string &name, eSGValueType types);
  SGNodeOutput* DefineOutput(const std::string &name, eSGValueType types);



private:
  std::string m_name;
  std::string m_key;
  std::string m_info;

  std::vector<SGNodeInput*>  m_inputs;
  std::vector<SGNodeOutput*> m_outputs;
};




}

