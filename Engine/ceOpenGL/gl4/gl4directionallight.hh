

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/idirectionallight.hh>

namespace ce::opengl
{


CS_CLASS()
class CS_OGL_API GL4DirectionalLight : public CS_SUPER(iDirectionalLight)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4DirectionalLight();
  ~GL4DirectionalLight() override = default;

  CS_NODISCARD CS_FORCEINLINE eLightType GetType() const override
  {
    return eLT_Directional;
  }

  void SetChangeMode(eLightChangeMode changeMode) override;
  eLightChangeMode GetChangeMode() const override;

  void SetCastShadow(bool castShadow) override;
  bool IsCastShadow() const;

  void SetShadowMapBias(float shadowBias) override;
  float GetShadowMapBias() const;

  void SetColor(const Color4f & color) override;
  const Color4f& GetColor() const override;

  void SetDirection(const Vector3f & direction) override;
  const Vector3f& GetDirection() const override;

  void SetIntensity(float intensity) override;
  float GetIntensity() const override;


private:

  eLightChangeMode m_changeMode;
  bool m_castShadow;
  float m_shadowBias;
  Color4f m_color;
  float m_intensity;

  Vector3f m_direction;

};

}
