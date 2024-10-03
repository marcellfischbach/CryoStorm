
#include <ceCore/graphics/shadergraph/csSGNodes.hh>

namespace cryo
{


/* *********************************************************/
/*     Const Float                                         */
/* *********************************************************/

const std::string csSGConstFloat::OUT_V = "V";
csSGConstFloat::csSGConstFloat()
    : csSGNode("Const Float")
{
  DefineInput("X", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Float);
}

void csSGConstFloat::SetValue(float x)
{
  GetInput(0)->SetScalar(x);
}

void csSGConstFloat::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Float);
}



/* *********************************************************/
/*     Const Vector 2                                      */
/* *********************************************************/

const std::string csSGConstVec2::OUT_V = "V";


csSGConstVec2::csSGConstVec2()
    : csSGNode("Const Vec2")
{
  DefineInput("X", eSGValueType::Invalid);
  DefineInput("Y", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Vector2);
}

void csSGConstVec2::SetValue(float x, float y)
{
  GetInput(0)->SetScalar(x);
  GetInput(1)->SetScalar(y);
}


void csSGConstVec2::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector2);
}


/* *********************************************************/
/*     Const Vector 3                                      */
/* *********************************************************/


const std::string csSGConstVec3::OUT_V = "V";
csSGConstVec3::csSGConstVec3()
    : csSGNode("Const Vec3")
{
  DefineInput("X", eSGValueType::Invalid);
  DefineInput("Y", eSGValueType::Invalid);
  DefineInput("Z", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Vector3);
}

void csSGConstVec3::SetValue(float x, float y, float z)
{
  GetInput(0)->SetScalar(x);
  GetInput(1)->SetScalar(y);
  GetInput(2)->SetScalar(z);
}

void csSGConstVec3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector3);

}

/* *********************************************************/
/*     Const Vector 4                                      */
/* *********************************************************/
const std::string csSGConstVec4::OUT_V = "V";

csSGConstVec4::csSGConstVec4()
    : csSGNode("Const Vec4")
{
  DefineInput("X", eSGValueType::Invalid);
  DefineInput("Y", eSGValueType::Invalid);
  DefineInput("Z", eSGValueType::Invalid);
  DefineInput("W", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Vector4);
}


void csSGConstVec4::SetValue(float x, float y, float z, float w)
{
  GetInput(0)->SetScalar(x);
  GetInput(1)->SetScalar(y);
  GetInput(2)->SetScalar(z);
  GetInput(3)->SetScalar(w);
}

void csSGConstVec4::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector4);
}




/* *********************************************************/
/*     Const Color 3                                       */
/* *********************************************************/

const std::string csSGConstColor3::OUT_V = "V";

csSGConstColor3::csSGConstColor3()
    : csSGNode("Const Color3")
{
  DefineInput("R", eSGValueType::Invalid);
  DefineInput("G", eSGValueType::Invalid);
  DefineInput("B", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Vector3);
}

void csSGConstColor3::SetValue(float r, float g, float b)
{
  GetInput(0)->SetScalar(r);
  GetInput(1)->SetScalar(g);
  GetInput(2)->SetScalar(b);
}

void csSGConstColor3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector3);
}



/* *********************************************************/
/*     Const Color 4                                       */
/* *********************************************************/

const std::string csSGConstColor4::OUT_V = "V";

csSGConstColor4::csSGConstColor4()
    : csSGNode("Const Color4")
{
  DefineInput("R", eSGValueType::Invalid);
  DefineInput("G", eSGValueType::Invalid);
  DefineInput("B", eSGValueType::Invalid);
  DefineInput("A", eSGValueType::Invalid);
  DefineOutput(OUT_V, eSGValueType::Vector3);
}

void csSGConstColor4::SetValue(float r, float g, float b, float a)
{
  GetInput(0)->SetScalar(r);
  GetInput(1)->SetScalar(g);
  GetInput(2)->SetScalar(b);
  GetInput(3)->SetScalar(a);
}

void csSGConstColor4::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector4);
}


/* *********************************************************/
/*     Vec 2                                               */
/* *********************************************************/

const std::string csSGVec2::IN_X  = "X";
const std::string csSGVec2::IN_Y  = "Y";
const std::string csSGVec2::OUT_V = "V";

csSGVec2::csSGVec2()
    : csSGNode("Vec2")
{
  DefineInput(IN_X, eSGValueType::Float);
  DefineInput(IN_Y, eSGValueType::Float);
  DefineOutput(OUT_V, eSGValueType::Vector2);
}

void csSGVec2::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector2);
}




/* *********************************************************/
/*     Vec 3                                               */
/* *********************************************************/

const std::string csSGVec3::IN_X  = "X";
const std::string csSGVec3::IN_Y  = "Y";
const std::string csSGVec3::IN_Z  = "Z";
const std::string csSGVec3::OUT_V = "V";

csSGVec3::csSGVec3()
    : csSGNode("Vec3")
{
  DefineInput(IN_X, eSGValueType::Float);
  DefineInput(IN_Y, eSGValueType::Float);
  DefineInput(IN_Z, eSGValueType::Float);
  DefineOutput(OUT_V, eSGValueType::Vector3);
}

void csSGVec3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector3);
}




/* *********************************************************/
/*     Vec 4                                               */
/* *********************************************************/

const std::string csSGVec4::IN_X  = "X";
const std::string csSGVec4::IN_Y  = "Y";
const std::string csSGVec4::IN_Z  = "Z";
const std::string csSGVec4::IN_W  = "W";
const std::string csSGVec4::OUT_V = "V";

csSGVec4::csSGVec4()
    : csSGNode("Vec4")
{
  DefineInput(IN_X, eSGValueType::Float);
  DefineInput(IN_Y, eSGValueType::Float);
  DefineInput(IN_Z, eSGValueType::Float);
  DefineInput(IN_W, eSGValueType::Float);
  DefineOutput(OUT_V, eSGValueType::Vector4);
}

void csSGVec4::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector3);
}

/* *********************************************************/
/*     Decompose Vec 2                                     */
/* *********************************************************/

const std::string csSGDecomposeVec2::IN_V  = "V";
const std::string csSGDecomposeVec2::OUT_X = "X";
const std::string csSGDecomposeVec2::OUT_Y = "Y";

csSGDecomposeVec2::csSGDecomposeVec2()
    : csSGNode("Decompose Vec2")
{
  DefineInput(IN_V, eSGValueType::Vector2);
  DefineOutput(OUT_X, eSGValueType::Float);
  DefineOutput(OUT_Y, eSGValueType::Float);
}

void csSGDecomposeVec2::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Float);
  GetOutput(1)->SetValueType(eSGValueType::Float);
}


/* *********************************************************/
/*     Decompose Vec 3                                     */
/* *********************************************************/

const std::string csSGDecomposeVec3::IN_V  = "V";
const std::string csSGDecomposeVec3::OUT_X = "X";
const std::string csSGDecomposeVec3::OUT_Y = "Y";
const std::string csSGDecomposeVec3::OUT_Z = "Z";

csSGDecomposeVec3::csSGDecomposeVec3()
    : csSGNode("Decompose Vec3")
{
  DefineInput(IN_V, eSGValueType::Vector3);
  DefineOutput(OUT_X, eSGValueType::Float);
  DefineOutput(OUT_Y, eSGValueType::Float);
  DefineOutput(OUT_Z, eSGValueType::Float);
}

void csSGDecomposeVec3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Float);
  GetOutput(1)->SetValueType(eSGValueType::Float);
  GetOutput(2)->SetValueType(eSGValueType::Float);
}


/* *********************************************************/
/*     Decompose Vec 4                                     */
/* *********************************************************/

const std::string csSGDecomposeVec4::IN_V  = "V";
const std::string csSGDecomposeVec4::OUT_X = "X";
const std::string csSGDecomposeVec4::OUT_Y = "Y";
const std::string csSGDecomposeVec4::OUT_Z = "Z";
const std::string csSGDecomposeVec4::OUT_W = "W";

csSGDecomposeVec4::csSGDecomposeVec4()
    : csSGNode("Decompose Vec4")
{
  DefineInput(IN_V, eSGValueType::Vector4);
  DefineOutput(OUT_X, eSGValueType::Float);
  DefineOutput(OUT_Y, eSGValueType::Float);
  DefineOutput(OUT_Z, eSGValueType::Float);
  DefineOutput(OUT_W, eSGValueType::Float);
}

void csSGDecomposeVec4::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Float);
  GetOutput(1)->SetValueType(eSGValueType::Float);
  GetOutput(2)->SetValueType(eSGValueType::Float);
  GetOutput(3)->SetValueType(eSGValueType::Float);
}




/* *********************************************************/
/*     Binary operator                                     */
/* *********************************************************/

const std::string csSGBinaryOperator::OUT_V = "V";

csSGBinaryOperator::csSGBinaryOperator(const std::string &name, eOperator op)
    : csSGNode(name)
    , m_op(op)
{
  DefineInput("A", eSGValueType::All);
  DefineInput("B", eSGValueType::All);
  DefineOutput(OUT_V, eSGValueType::All);
}


csSGBinaryOperator::eOperator csSGBinaryOperator::GetOperator() const
{
  return m_op;
}

void csSGBinaryOperator::CalcIOTypes()
{
  auto s0  = GetInput(0)->GetInputValueType();
  auto s1  = GetInput(1)->GetInputValueType();
  auto out = GetOutput(0);

  out->SetValueType(EvalValueType(s0, s1));
}

csSGAdd::csSGAdd() : csSGBinaryOperator("Add", eOperator::Add)
{
}

csSGSub::csSGSub() : csSGBinaryOperator("Sub", eOperator::Sub)
{
}

csSGMul::csSGMul() : csSGBinaryOperator("Mul", eOperator::Mul)
{
}

csSGDiv::csSGDiv() : csSGBinaryOperator("Div", eOperator::Div)
{
}

csSGMod::csSGMod() : csSGBinaryOperator("Mod", eOperator::Mod)
{
}





/* *********************************************************/
/*     Streaming input                                     */
/* *********************************************************/


csSGStreamNode::csSGStreamNode(const std::string &name, eVertexStream stream, eSGValueType type)
    : csSGNode(name)
    , m_stream(stream)
    , m_type(type)
{
  DefineOutput("V", type);
}

void csSGStreamNode::CalcIOTypes()
{
  GetOutput(0)->SetValueType(m_type);
}

eVertexStream csSGStreamNode::GetStream() const
{
  return m_stream;
}


csSGTexCoord::csSGTexCoord()
    : csSGStreamNode("TexCoord", eVS_UV, eSGValueType::Vector2)
{

}


csSGResourceNode::csSGResourceNode(const std::string &name, eMaterialAttributeType matType)
    : csSGNode(name)
    , m_matType(matType)
{

}


void csSGResourceNode::SetResourceName(const std::string &name)
{
  m_resourceName = name;
}

const std::string &csSGResourceNode::GetResourceName() const
{
  return m_resourceName;
}

eMaterialAttributeType csSGResourceNode::GetMatType() const
{
  return m_matType;
}


csSGResourceFloat::csSGResourceFloat()
: csSGResourceNode("Resource Float", eMAT_Float)
{
  DefineOutput("V", eSGValueType::Float);
}

void csSGResourceFloat::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Float);
}


csSGResourceColor3::csSGResourceColor3()
    : csSGResourceNode("Resource Color3", eMAT_Vec3)
{
  DefineOutput("V", eSGValueType::Vector3);
}

void csSGResourceColor3::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector3);
}


csSGTexture1D::csSGTexture1D()
    : csSGResourceNode("Texture1D", eMAT_Texture)
{
  DefineInput("UV", eSGValueType::Float);
  DefineOutput("Color", eSGValueType::Vector4);
}


void csSGTexture1D::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector4);
}

csSGTexture2D::csSGTexture2D()
    : csSGResourceNode("Texture2D", eMAT_Texture)
{
  DefineInput("UV", eSGValueType::Vector2);
  DefineOutput("Color", eSGValueType::Vector4);
}


void csSGTexture2D::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector4);
}

csSGTexture3D::csSGTexture3D()
    : csSGResourceNode("Texture3D", eMAT_Texture)
{
  DefineInput("UV", eSGValueType::Vector3);
  DefineOutput("Color", eSGValueType::Vector4);
}


void csSGTexture3D::CalcIOTypes()
{
  GetOutput(0)->SetValueType(eSGValueType::Vector4);
}


} // ce