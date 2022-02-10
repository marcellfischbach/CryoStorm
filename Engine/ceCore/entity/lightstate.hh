
#pragma  once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/graphics/elighttype.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{

struct iLight;
struct iPointLight;
struct iDirectionalLight;
class GfxLight;
class World;

CE_CLASS()
class CE_CORE_API LightState : public CE_SUPER(SpatialState)
{
  CE_CLASS_GEN_OBJECT;

public:
  LightState(const std::string &name);
  ~LightState() override;

  void SetType(eLightType type);
  CE_NODISCARD eLightType GetType() const;

  void SetCastShadow(bool castShadow);
  CE_NODISCARD bool IsCastShadow() const;

  void SetShadowMapBias(float bias);
  CE_NODISCARD float GetShadowMapBias() const;

  void SetColor(const Color4f &color);
  CE_NODISCARD const Color4f &GetColor() const;

  void SetRange(float range);
  CE_NODISCARD float GetRange() const;


  void OnAttachedToWorld(World * world) override;
  void OnDetachedFromWorld(World * world) override;

protected:
  void TransformationUpdatedPreChildren() override;

private:
  iLight *CreateLight();
  void AddToScene(World *world);
  void RemoveFromScene(World * world);

  void UpdateValues();


  iLight* m_light;
  iPointLight* m_pointLight;
  iDirectionalLight* m_directionalLight;

  GfxLight *m_gfxLight;

  eLightType m_lightType;

  bool m_castShadow;
  float m_shadowBias;
  Color4f m_color;
  float m_range;
};

}

