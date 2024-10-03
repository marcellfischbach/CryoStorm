

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/ipointlight.hh>

namespace ce::opengl
{


CS_CLASS()
class CS_OGL_API GL4PointLight : public CS_SUPER(iPointLight)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4PointLight();
  virtual ~GL4PointLight();

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

  void SetColor(const Color4f & color) override;
  const Color4f& GetColor() const override;

  void SetIntensity(float intensity) override;
  float GetIntensity() const override;

  void SetPosition(const Vector3f & position) override;
  const Vector3f& GetPosition() const override;

  void SetRange(float range) override;
  float GetRange() const override;

private:

  eLightChangeMode m_changeMode;
  bool m_castShadow;
  float m_shadowBias;
  Color4f m_color;
  float m_intensity;

  Vector3f m_position;
  float m_range;

};

}