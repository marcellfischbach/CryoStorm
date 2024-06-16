
#include <ceCore/graphics/shadergraph/sgnodes.hh>

namespace ce
{

SGConstVec2::SGConstVec2()
: SGNode("Const Vec2")
{
  DefineInput("X", eSGValueType::Invalid);
  DefineInput("Y", eSGValueType::Invalid);
  DefineOutput("V", eSGValueType::Vector2);
}

void SGConstVec2::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector2);
}


SGConstColor3::SGConstColor3()
  : SGNode("Const Color3")
{
  DefineInput("R", eSGValueType::Invalid);
  DefineInput("G", eSGValueType::Invalid);
  DefineInput("B", eSGValueType::Invalid);
  DefineOutput("V", eSGValueType::Color3);
}

void SGConstColor3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Color3);
}

SGAdd::SGAdd()
    : SGNode("Add")
{
  DefineInput("A", eSGValueType::All);
  DefineInput("B", eSGValueType::All);
  DefineOutput("V", eSGValueType::All);
}


void SGAdd::CalcIOTypes()
{
  auto s0 = GetInput(0)->GetInputValueType();
  auto s1 = GetInput(1)->GetInputValueType();
  auto out = GetOutput(0);

  out->SetValueType(EvalValueType(s0, s1));
}

} // ce