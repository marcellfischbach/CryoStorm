
#pragma  once


#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>
#include <spcCore/graphics/elighttype.hh>
#include <spcCore/math/color4f.hh>

namespace spc
{

struct iLight;
struct iPointLight;
struct iDirectionalLight;
class World;

SPC_CLASS()
class SPC_CORE_API LightState : public SPC_SUPER(SpatialState)
{
  SPC_CLASS_GEN;

public:
  LightState(const std::string &name);
  ~LightState() override;

  void SetType(eLightType type);
  eLightType GetType() const;

  void SetCastShadow(bool castShadow);
  bool IsCastShadow() const;

  void SetColor(const Color4f &color);
  const Color4f &GetColor() const;

  void SetRange(float range);
  float GetRange() const;

  void OnAttachedToWorld(World * world) override;
  void OnDetachedFromWorld(World * world) override;

protected:
  void TransformationUpdated() override;

private:
  iLight *CreateLight();
  void AddToScene(World *world);
  void RemoveFromScene(World * world);

  void UpdateValues();


  iLight* m_light;
  iPointLight* m_pointLight;
  iDirectionalLight* m_directionalLight;

  eLightType m_lightType;

  bool m_castShadow;
  Color4f m_color;
  float m_range;
};

}

