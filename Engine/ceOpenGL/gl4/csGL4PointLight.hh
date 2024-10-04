

#pragma once

#include <ceOpenGL/csOpenGLExport.hh>
#include <ceCore/graphics/iPointLight.hh>

namespace cryo::opengl
{


CS_CLASS()
class CS_OGL_API csGL4PointLight : public CS_SUPER(iPointLight)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4PointLight();
  virtual ~csGL4PointLight();

  eLightType GetType() const
  {
    return eLT_Point;
  }


  void SetChangeMode(eLightChangeMode changeMode) override;
  eLightChangeMode GetChangeMode() const override;

  void SetCastShadow(bool castShadow) override;
  bool IsCastShadow() const;

  void SetShadowMapBias(float shadowBias) override;
  float GetShadowMapBias() const;

  void SetColor(const csColor4f & color) override;
  const csColor4f& GetColor() const override;

  void SetIntensity(float intensity) override;
  float GetIntensity() const override;

  void SetPosition(const csVector3f & position) override;
  const csVector3f& GetPosition() const override;

  void SetRange(float range) override;
  float GetRange() const override;

private:

  eLightChangeMode m_changeMode;
  bool m_castShadow;
  float     m_shadowBias;
  csColor4f m_color;
  float     m_intensity;

  csVector3f m_position;
  float      m_range;

};

}