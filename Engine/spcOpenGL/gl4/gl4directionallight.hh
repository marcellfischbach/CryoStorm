

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/idirectionallight.hh>

namespace spc::opengl
{


SPC_CLASS()
class SPC_OGL_API GL4DirectionalLight : public SPC_SUPER(iDirectionalLight)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4DirectionalLight();
  ~GL4DirectionalLight() override = default;

  SPC_NODISCARD SPC_FORCEINLINE eLightType GetType() const override
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

  void SetSplits(float split0, float split1, float split2) override;
  SPC_NODISCARD float GetSplit0() const override;
  SPC_NODISCARD float GetSplit1() const override;
  SPC_NODISCARD float GetSplit2() const override;

private:

  eLightChangeMode m_changeMode;
  bool m_castShadow;
  float m_shadowBias;
  Color4f m_color;
  float m_intensity;
  Vector3f m_split;

  Vector3f m_direction;

};

}
