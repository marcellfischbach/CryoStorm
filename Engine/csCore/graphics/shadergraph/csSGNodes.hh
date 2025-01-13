
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/material/eMaterialAttributeType.hh>
#include <csCore/graphics/eVertexStream.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <array>

namespace cs
{

CS_CLASS()
class CS_CORE_API csSGConstFloat : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstFloat();
  ~csSGConstFloat() override = default;

  void SetValue(float v);

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGConstVec2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstVec2();
  ~csSGConstVec2() override = default;

  void SetValue(float x, float y);

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGConstVec3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstVec3();
  ~csSGConstVec3() override = default;

  void SetValue(float x, float y, float z);

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGConstVec4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstVec4();
  ~csSGConstVec4() override = default;

  void SetValue(float x, float y, float z, float w);

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGConstColor3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstColor3();
  ~csSGConstColor3() override = default;

  void CalcIOTypes() override;
  void SetValue(float r, float g, float b);

};

CS_CLASS()
class CS_CORE_API csSGConstColor4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string OUT_V;

  csSGConstColor4();
  ~csSGConstColor4() override = default;

  void CalcIOTypes() override;
  void SetValue(float r, float g, float b, float a);

};


CS_CLASS()
class CS_CORE_API csSGVec2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string OUT_V;

  csSGVec2();
  ~csSGVec2() override = default;

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGVec3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string IN_Z;
  const static std::string OUT_V;

  csSGVec3();
  ~csSGVec3() override = default;

  void CalcIOTypes() override;
};


CS_CLASS()
class CS_CORE_API csSGVec4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_X;
  const static std::string IN_Y;
  const static std::string IN_Z;
  const static std::string IN_W;
  const static std::string OUT_V;

  csSGVec4();
  ~csSGVec4() override = default;

  void CalcIOTypes() override;
};

CS_CLASS()
class CS_CORE_API csSGTransformVec4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_V;
  const static std::string OUT_V2;
  const static std::string OUT_V3;

  csSGTransformVec4();
  ~csSGTransformVec4() override = default;

  void CalcIOTypes() override;
};



CS_CLASS()
class CS_CORE_API csSGDecomposeVec2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;

  csSGDecomposeVec2();
  ~csSGDecomposeVec2() override = default;

  void CalcIOTypes() override;

};


CS_CLASS()
class CS_CORE_API csSGDecomposeVec3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;

  csSGDecomposeVec3();
  ~csSGDecomposeVec3() override = default;

  void CalcIOTypes() override;

};


CS_CLASS()
class CS_CORE_API csSGDecomposeVec4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  const static std::string IN_V;
  const static std::string OUT_X;
  const static std::string OUT_Y;
  const static std::string OUT_Z;
  const static std::string OUT_W;

  csSGDecomposeVec4();
  ~csSGDecomposeVec4() override = default;

  void CalcIOTypes() override;

};


CS_CLASS()
class CS_CORE_API csSGBinaryOperator : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
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

  ~csSGBinaryOperator() override = default;

  eOperator GetOperator() const;
  void CalcIOTypes() override;

protected:
  csSGBinaryOperator(const std::string & name, eOperator op);


private:
  eOperator m_op;

};


CS_CLASS()
class CS_CORE_API csSGAdd : public CS_SUPER(csSGBinaryOperator)
{
  CS_CLASS_GEN;
public:
  csSGAdd();
  ~csSGAdd() override = default;
};


CS_CLASS()
class CS_CORE_API csSGSub : public CS_SUPER(csSGBinaryOperator)
{
  CS_CLASS_GEN;
public:
  csSGSub();
  ~csSGSub() override = default;
};


CS_CLASS()
class CS_CORE_API csSGMul : public CS_SUPER(csSGBinaryOperator)
{
  CS_CLASS_GEN;
public:
  csSGMul();
  ~csSGMul() override = default;
};


CS_CLASS()
class CS_CORE_API csSGDiv : public CS_SUPER(csSGBinaryOperator)
{
  CS_CLASS_GEN;
public:
  csSGDiv();
  ~csSGDiv() override = default;
};


CS_CLASS()
class CS_CORE_API csSGMod : public CS_SUPER(csSGBinaryOperator)
{
  CS_CLASS_GEN;
public:
  csSGMod();
  ~csSGMod() override = default;
};


CS_CLASS()
class CS_CORE_API csSGStreamNode : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  ~csSGStreamNode() override = default;

  eVertexStream GetStream() const;

  void CalcIOTypes() override;

protected:
  csSGStreamNode(const std::string & name, eVertexStream stream, eSGValueType type);

private:
  eVertexStream m_stream;
  eSGValueType  m_type;

};

CS_CLASS()
class CS_CORE_API csSGTexCoord : public CS_SUPER(csSGStreamNode)
{
  CS_CLASS_GEN;
public:
  csSGTexCoord();
  ~csSGTexCoord() override = default;
};

CS_CLASS()
class CS_CORE_API csSGResourceNode : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
protected:
  csSGResourceNode(const std::string & name, eMaterialAttributeType matType);

public:
  ~csSGResourceNode() override = default;

  void SetResourceName(const std::string & name);
  const std::string& GetResourceName() const;

  eMaterialAttributeType GetMatType() const;

  void SetDefault(const std::array<float, 16> &v);
  void SetDefault(const std::array<int, 4> &v);
  void SetDefault(const csAssetLocator &locator);

  const std::array<float, 16>& GetDefaultFloats() const;
  const std::array<int, 4>& GetDefaultInts() const;
  const csAssetLocator &GetDefaultLocator() const;

private:
  eMaterialAttributeType m_matType;
  std::string            m_resourceName;

  std::array<int, 4> m_defaultInts;
  std::array<float, 16> m_defaultFloats;
  csAssetLocator        m_defaultLocator;

};

CS_CLASS()
class CS_CORE_API csSGResourceFloat : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGResourceFloat();
  ~csSGResourceFloat() override = default;

  void CalcIOTypes() override;

};

CS_CLASS()
class CS_CORE_API csSGResourceColor3 : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGResourceColor3();
  ~csSGResourceColor3() override = default;

  void CalcIOTypes() override;

};



CS_CLASS()
class CS_CORE_API csSGTexture1D : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGTexture1D();
  ~csSGTexture1D() override = default;

  void CalcIOTypes() override;

};


CS_CLASS()
class CS_CORE_API csSGTexture2D : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGTexture2D();
  ~csSGTexture2D() override = default;

  void CalcIOTypes() override;

};


CS_CLASS()
class CS_CORE_API csSGTexture3D : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGTexture3D();
  ~csSGTexture3D() override = default;

  void CalcIOTypes() override;

};


} // ce
