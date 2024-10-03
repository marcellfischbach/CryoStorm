
#pragma  once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/graphics/elighttype.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/vector3f.hh>

namespace cryo
{

struct iLight;
struct iPointLight;
struct iDirectionalLight;
class GfxLight;
class World;

CS_CLASS()
class CS_CORE_API LightState : public CS_SUPER(SpatialState)
{
  CS_CLASS_GEN_OBJECT;

public:
  LightState(const std::string &name);
  ~LightState() override;

  void SetType(eLightType type);
  CS_NODISCARD eLightType GetType() const;

  void SetCastShadow(bool castShadow);
  CS_NODISCARD bool IsCastShadow() const;

  void SetShadowMapBias(float bias);
  CS_NODISCARD float GetShadowMapBias() const;

  void SetColor(const Color4f &color);
  CS_NODISCARD const Color4f &GetColor() const;

  void SetRange(float range);
  CS_NODISCARD float GetRange() const;


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

