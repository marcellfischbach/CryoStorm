
#include <csCore/graphics/shadergraph/csSGNode.hh>


namespace cs
{



csSGNode::csSGNode(const std::string &name)
    : m_name(name)
{

}

void csSGNode::SetKey(const std::string &key)
{
  m_key = key;
}

const std::string &csSGNode::GetKey() const
{
  return m_key;
}

const std::string& csSGNode::GetName() const
{
  return m_name;
}

csSGNodeInput *csSGNode::DefineInput(const std::string &name, eSGValueType types)
{
  auto input = new csSGNodeInput(this, name);
  input->SetTypes(types);
  m_inputs.push_back(input);
  return input;
}

csSGNodeOutput *csSGNode::DefineOutput(const std::string &name, eSGValueType types)
{
  auto output = new csSGNodeOutput(this, name);
  output->SetTypes(types);
  m_outputs.push_back(output);
  return output;
}

void csSGNode::Bind(size_t inputIdx, cs::csSGNode *node, size_t outputIdx)
{
  if (inputIdx >= m_inputs.size())
  {
    return;
  }

  csSGNodeInput  *input     = m_inputs[inputIdx];
  csSGNodeOutput *oldSource = input->GetSource();

  if (!node)
  {
    if (oldSource)
    {
      oldSource->Remove(input);
    }
    input->SetSource(nullptr);
  }
  else
  {
    csSGNodeOutput *newSource = node->GetOutput(outputIdx);
    if (newSource == oldSource)
    {
      return;
    }
    if (oldSource)
    {
      oldSource->Remove(input);
    }
    if (newSource)
    {
      input->SetSource(newSource);
      newSource->Add(input);
    }
  }
  
}

size_t csSGNode::GetNumberOfInputs() const
{
  return m_inputs.size();
}

csSGNodeInput* csSGNode::GetInput(size_t idx)
{
  if (idx >= m_inputs.size())
  {
    return nullptr;
  }
  return m_inputs[idx];
}


const csSGNodeInput* csSGNode::GetInput(size_t idx) const
{
  if (idx >= m_inputs.size())
  {
    return nullptr;
  }
  return m_inputs[idx];
}

int csSGNode::IndexOfInput(const std::string &inputName) const
{
  for (int i=0, in=m_inputs.size(); i<in; i++)
  {
    if (m_inputs[i]->GetName() == inputName)
    {
      return i;
    }
  }
  return -1;
}

csSGNodeOutput* csSGNode::GetOutput(size_t idx)
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }
  return m_outputs[idx];
}


size_t csSGNode::GetNumberOfOutputs() const
{
  return m_outputs.size();
}

const csSGNodeOutput* csSGNode::GetOutput(size_t idx) const
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }
  return m_outputs[idx];
}

int csSGNode::IndexOfOutput(const std::string &inputName) const
{
  for (int i=0, in=m_outputs.size(); i<in; i++)
  {
    if (m_outputs[i]->GetName() == inputName)
    {
      return i;
    }
  }
  return -1;
}

void csSGNode::SetPosition(const cs::csVector2f &pos)
{
  m_position = pos;
}

const csVector2f &csSGNode::GetPosition() const
{
  return m_position;
}

eSGValueType csSGNodeInput::GetInputValueType() const
{
  if (m_source)
  {
    return m_source->GetValueType();
  }
  return GetTypes();
}


eSGValueType EvalValueType (eSGValueType v0, eSGValueType v1)
{
  if (v0 == v1)
  {
    return v0;
  }

  if (v0 == eSGValueType::Float)
  {
    return v1;
  }

  if (v1 == eSGValueType::Float)
  {
      return v0;
  }
  return eSGValueType::Invalid;
}

}