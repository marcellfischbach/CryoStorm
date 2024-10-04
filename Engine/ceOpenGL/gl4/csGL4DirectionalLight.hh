

#pragma once

#include <ceOpenGL/csOpenGLExport.hh>
#include <ceCore/graphics/iDirectionalLight.hh>

namespace cryo::opengl
{


CS_CLASS()
class CS_OGL_API csGL4DirectionalLight : public CS_SUPER(iDirectionalLight)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4DirectionalLight();
  ~csGL4DirectionalLight() override = default;

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

  void SetColor(const csColor4f & color) override;
  const csColor4f& GetColor() const override;

  void SetDirection(const csVector3f & direction) override;
  const csVector3f& GetDirection() const override;

  void SetIntensity(float intensity) override;
  float GetIntensity() const override;


private:

  eLightChangeMode m_changeMode;
  bool m_castShadow;
  float     m_shadowBias;
  csColor4f m_color;
  float     m_intensity;

  csVector3f m_direction;

};

}
