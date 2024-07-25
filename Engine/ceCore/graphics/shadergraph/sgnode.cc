
#include <ceCore/graphics/shadergraph/sgnode.hh>


namespace ce
{



SGNode::SGNode( const std::string &name)
    : m_name(name)
{

}

void SGNode::SetKey(const std::string &key)
{
  m_key = key;
}

const std::string &SGNode::GetKey () const
{
  return m_key;
}

const std::string& SGNode::GetName() const
{
  return m_name;
}

SGNodeInput *SGNode::DefineInput(const std::string &name, eSGValueType types)
{
  auto input = new SGNodeInput(this, name);
  input->SetTypes(types);
  m_inputs.push_back(input);
  return input;
}

SGNodeOutput *SGNode::DefineOutput(const std::string &name, eSGValueType types)
{
  auto output = new SGNodeOutput(this, name);
  output->SetTypes(types);
  m_outputs.push_back(output);
  return output;
}

void SGNode::Bind(size_t inputIdx, ce::SGNode *node, size_t outputIdx)
{
  if (inputIdx >= m_inputs.size())
  {
    return;
  }

  SGNodeInput *input = m_inputs[inputIdx];
  SGNodeOutput *oldSource = input->GetSource();

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
    SGNodeOutput *newSource = node->GetOutput(outputIdx);
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

size_t SGNode::GetNumberOfInputs() const
{
  return m_inputs.size();
}

SGNodeInput* SGNode::GetInput(size_t idx)
{
  if (idx >= m_inputs.size())
  {
    return nullptr;
  }
  return m_inputs[idx];
}


const SGNodeInput* SGNode::GetInput(size_t idx) const
{
  if (idx >= m_inputs.size())
  {
    return nullptr;
  }
  return m_inputs[idx];
}

int SGNode::IndexOfInput(const std::string &inputName) const
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

SGNodeOutput* SGNode::GetOutput(size_t idx)
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }
  return m_outputs[idx];
}


size_t SGNode::GetNumberOfOutputs() const
{
  return m_outputs.size();
}

const SGNodeOutput* SGNode::GetOutput(size_t idx) const
{
  if (idx >= m_outputs.size())
  {
    return nullptr;
  }
  return m_outputs[idx];
}

int SGNode::IndexOfOutput(const std::string &inputName) const
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

eSGValueType SGNodeInput::GetInputValueType() const
{
  if (m_source)
  {
    return m_source->GetValueType();
  }
  return eSGValueType::Float;
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