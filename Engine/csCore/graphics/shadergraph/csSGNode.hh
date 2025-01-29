#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/graphics/shadergraph/eSGValueType.hh>
#include <csCore/math/csVector.hh>
#include <utility>
#include <string>
#include <vector>

namespace cs
{

eSGValueType EvalValueType(eSGValueType v0, eSGValueType v1);

class csSGNode;
class csSGNodeInput;
class csSGNodeOutput;

CS_CLASS()
class CS_CORE_API csSGNodeIO : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  CS_NODISCARD const csSGNode* GetNode() const
  {
    return m_node;
  }
  CS_NODISCARD csSGNode* GetNode()
  {
    return m_node;
  }

  CS_NODISCARD const std::string& GetName() const
  {
    return m_name;
  }

  void SetTypes(eSGValueType types)
  {
    m_types = types;
  }
  CS_NODISCARD eSGValueType GetTypes() const
  {
    return m_types;
  }

protected:
  csSGNodeIO(csSGNode * node, std::string name) : m_node(node), m_name(std::move(name))
  {
  }
  virtual ~csSGNodeIO()
  {
    m_node = nullptr;
  }

private:

  csSGNode *m_node;
  std::string m_name;
  eSGValueType m_types = eSGValueType::Invalid;

};

CS_CLASS()
class CS_CORE_API csSGNodeOutput : public CS_SUPER(csSGNodeIO)
{
  CS_CLASS_GEN;
public:
  csSGNodeOutput(csSGNode * node, const std::string & name) : csSGNodeIO(node, name)
  {
  }
  ~csSGNodeOutput() override
  {
    m_destinations.clear();
  }


  void Add(csSGNodeInput* input);
  void Remove(csSGNodeInput* input);
  CS_NODISCARD const std::vector<csRef<csSGNodeInput>>& GetInputs() const
  {
    return m_destinations;
  }

  CS_NODISCARD eSGValueType GetValueType() const
  {
    return m_valueType;
  }
  void SetValueType(eSGValueType valueType)
  {
    m_valueType = valueType;
  }
protected:

private:
  eSGValueType              m_valueType = eSGValueType::Invalid;
  std::vector<csRef<csSGNodeInput>> m_destinations;
};

CS_CLASS()
class CS_CORE_API csSGNodeInput : public CS_SUPER(csSGNodeIO)
{
  CS_CLASS_GEN;
public:

  enum eModifiable {
    eM_Modifiable,
    eM_NoValue,
    eM_Const,
  };

  csSGNodeInput(csSGNode * node, const std::string & name, eModifiable modifiable) : csSGNodeIO(node, name), m_scalar(0.0f), m_modifiable(modifiable)
  {
  }
  ~csSGNodeInput() override = default;

  void SetSource(csSGNodeOutput* output);
  CS_NODISCARD csSGNodeOutput* GetSource()
  {
    return m_source;
  }
  CS_NODISCARD const csSGNodeOutput* GetSource() const
  {
    return m_source;
  }
  CS_NODISCARD float GetScalar() const
  {
    return m_scalar;
  }
  void SetScalar(float scalar)
  {
    m_scalar = scalar;
  }

  bool IsConst()
  {
    return m_modifiable == eM_Const;
  }

  bool IsModifiable()
  {
    return m_modifiable == eM_Modifiable;
  }

  bool IsNoValue()
  {
    return m_modifiable == eM_NoValue;
  }



  eSGValueType GetInputValueType() const;

private:
  csRef<csSGNodeOutput> m_source = nullptr;
  float m_scalar;

  eModifiable m_modifiable;
};


CS_CLASS()
class CS_CORE_API csSGNode : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;

public:
  ~csSGNode() override = default;

  void SetKey(const std::string & key);
  const std::string& GetKey() const;

  const std::string& GetName() const;

  virtual void CalcIOTypes() = 0;

  void Bind(size_t inputIdx, csSGNode * node, size_t outputIdx = 0);

  size_t GetNumberOfInputs() const;
  csSGNodeInput* GetInput(size_t idx);
  const csSGNodeInput* GetInput(size_t idx) const;
  int IndexOfInput(const std::string & inputName) const;

  size_t GetNumberOfOutputs() const;
  csSGNodeOutput* GetOutput(size_t idx = 0);
  const csSGNodeOutput* GetOutput(size_t idx = 0) const;
  int IndexOfOutput(const std::string & inputName) const;

  void SetPosition(const csVector2f & pos);
  const csVector2f& GetPosition() const;

protected:
  explicit csSGNode(const std::string & name);

  csSGNodeInput* DefineInput(const std::string & name, eSGValueType types, csSGNodeInput::eModifiable modifiable);
  csSGNodeOutput* DefineOutput(const std::string & name, eSGValueType types);



private:
  std::string m_name;
  std::string m_key;

  std::vector<csRef<csSGNodeInput>>  m_inputs;
  std::vector<csRef<csSGNodeOutput>> m_outputs;

  csVector2f m_position;
};




}

