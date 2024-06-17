
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>
#include <ceCore/graphics/evertexstream.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API SGConstFloat : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;

  SGConstFloat();
  ~SGConstFloat() override = default;

  void SetValue(float v);

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGConstVec2 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;

  SGConstVec2();
  ~SGConstVec2() override = default;

  void SetValue(float x, float y);

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGConstVec3 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;

  SGConstVec3();
  ~SGConstVec3() override = default;

  void SetValue(float x, float y, float z);

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGConstVec4 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;
  const static std::string OUT_W;

  SGConstVec4();
  ~SGConstVec4() override = default;

  void SetValue(float x, float y, float z, float w);

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGConstColor3 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  const static std::string OUT_R;
  const static std::string OUT_G;
  const static std::string OUT_B;

  SGConstColor3();
  ~SGConstColor3() override = default;

  void CalcIOTypes() override;
  void SetValue(float r, float g, float b);

};

CE_CLASS()
class CE_CORE_API SGConstColor4 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  const static std::string OUT_R;
  const static std::string OUT_G;
  const static std::string OUT_B;
  const static std::string OUT_A;

  SGConstColor4();
  ~SGConstColor4() override = default;

  void CalcIOTypes() override;
  void SetValue(float r, float g, float b, float a);

};


CE_CLASS()
class CE_CORE_API SGVec2 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;

  SGVec2();
  ~SGVec2() override = default;

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGVec3 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string IN_Z;
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;

  SGVec3();
  ~SGVec3() override = default;

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGVec4 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string IN_Z;
  const static std::string IN_W;
  const static std::string OUT_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;
  const static std::string OUT_W;

  SGVec4();
  ~SGVec4() override = default;

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGDecomposeVec2 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_X;
  const static std::string OUT_Y;

  SGDecomposeVec2();
  ~SGDecomposeVec2() override = default;

  void CalcIOTypes() override;

};


CE_CLASS()
class CE_CORE_API SGBinaryOperator : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  const static std::string OUT_V;
  enum eOperator
  {
    Add,
    Sub,
    Mul,
    Div,
    Mod
  };

  ~SGBinaryOperator() override = default;

  eOperator GetOperator() const;
  void CalcIOTypes() override;

protected:
  SGBinaryOperator(const std::string &name, eOperator op);


private:
  eOperator m_op;

};


CE_CLASS()
class CE_CORE_API SGAdd : public CE_SUPER(SGBinaryOperator)
{
CE_CLASS_GEN;
public:
  SGAdd();
  ~SGAdd() override = default;
};


CE_CLASS()
class CE_CORE_API SGSub : public CE_SUPER(SGBinaryOperator)
{
CE_CLASS_GEN;
public:
  SGSub();
  ~SGSub() override = default;
};


CE_CLASS()
class CE_CORE_API SGMul : public CE_SUPER(SGBinaryOperator)
{
CE_CLASS_GEN;
public:
  SGMul();
  ~SGMul() override = default;
};


CE_CLASS()
class CE_CORE_API SGDiv : public CE_SUPER(SGBinaryOperator)
{
CE_CLASS_GEN;
public:
  SGDiv();
  ~SGDiv() override = default;
};


CE_CLASS()
class CE_CORE_API SGMod : public CE_SUPER(SGBinaryOperator)
{
CE_CLASS_GEN;
public:
  SGMod();
  ~SGMod() override = default;
};


CE_CLASS()
class CE_CORE_API SGStreamNode : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  ~SGStreamNode() override = default;

  eVertexStream GetStream() const;

  void CalcIOTypes() override;

protected:
  SGStreamNode(const std::string &name, eVertexStream stream, eSGValueType type);

private:
  eVertexStream m_stream;
  eSGValueType  m_type;

};

CE_CLASS()
class CE_CORE_API SGTexCoord : public CE_SUPER(SGStreamNode)
{
CE_CLASS_GEN;
public:
  SGTexCoord();
  ~SGTexCoord() override = default;
};


} // ce
