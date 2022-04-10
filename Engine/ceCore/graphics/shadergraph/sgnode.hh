#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/shadergraph/esgvaluetype.hh>
#include <utility>
#include <ceCore/math/vector.hh>
#include <string>
#include <set>
#include <vector>

namespace ce
{

class SGNode;
class SGNodeInput;
class SGNodeOutput;

class CE_CORE_API SGNodeIO
{
public:
  CE_NODISCARD const SGNode *GetNode() const
  { return m_node; }
  CE_NODISCARD SGNode *GetNode()
  { return m_node; }

  CE_NODISCARD const std::string &GetName() const
  { return m_name; }

  void SetType(eSGValueType type)
  { m_type = type; }
  CE_NODISCARD eSGValueType GetType() const
  { return m_type; }

protected:
  SGNodeIO(SGNode *node, std::string name) : m_node(node), m_name(std::move(name))
  {}
  virtual ~SGNodeIO()
  { m_node = nullptr; }

private:

  SGNode *m_node;
  std::string m_name;
  eSGValueType m_type = eSGValueType::Invalid;

};

class CE_CORE_API SGNodeOutput : public SGNodeIO
{
public:
  SGNodeOutput(SGNode *node, const std::string &name) : SGNodeIO(node, name)
  {}
  ~SGNodeOutput() override
  { m_inputs.clear(); }


  void add(SGNodeInput *input)
  { m_inputs.insert(input); }
  void remove(SGNodeInput *input)
  { m_inputs.erase(input); }
  CE_NODISCARD const std::set<SGNodeInput *> &GetInputs() const
  { return m_inputs; }
private:
  std::set<SGNodeInput *> m_inputs;
};

class CE_CORE_API SGNodeInput : public SGNodeIO
{
public:
  SGNodeInput(SGNode *node, const std::string &name) : SGNodeIO(node, name)
  {}
  ~SGNodeInput() override = default;

  void SetOutput(SGNodeOutput *output)
  { m_output = output; }
  CE_NODISCARD SGNodeOutput *GetOutput()
  { return m_output; }
  CE_NODISCARD const SGNodeOutput *GetOutput() const
  { return m_output; }

private:
  SGNodeOutput *m_output = nullptr;
};


class CE_CORE_API SGNode
{
public:
  virtual ~SGNode();


protected:
  SGNode(const std::string &key, const std::string &name);

  void defineInput(const std::string &name, eSGValueType bindTypes, eSGValueType valueType);
  void defineOutput(const std::string &name, eSGValueType type);

  virtual void calcIOTypes() = 0;

private:
  std::string m_key;
  std::string m_name;
};




}

