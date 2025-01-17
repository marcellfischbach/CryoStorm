
#pragma  once


#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/graphics/eLightType.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/csRef.hh>


namespace cs
{


struct iLight;
struct iPointLight;
struct iDirectionalLight;
class csGfxLight;
class csWorld;

CS_CLASS(jclass="org.cryo.core.entity.LightState")
class CS_CORE_API csLightState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN_OBJECT;

public:
  csLightState(const std::string &name = std::string (""));
  ~csLightState() override;

  CS_FUNCTION(jenum)
  void SetType(cs::eLightType type);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eLightType GetType() const;

  CS_FUNCTION()
  void SetCastShadow(bool castShadow);
  CS_FUNCTION()
  CS_NODISCARD  bool IsCastShadow() const;

  CS_FUNCTION()
  void SetShadowMapBias(float bias);
  CS_FUNCTION()
  CS_NODISCARD float GetShadowMapBias() const;

  CS_FUNCTION()
  void SetColor(const cs::csColor4f &color);

  CS_FUNCTION()
  CS_NODISCARD const cs::csColor4f &GetColor() const;

  CS_FUNCTION()
  void SetRange(float range);
  CS_FUNCTION()
  CS_NODISCARD float GetRange() const;


  void OnAttachedToWorld(csWorld * world) override;
  void OnDetachedFromWorld(csWorld * world) override;

protected:
  void TransformationUpdatedPreChildren() override;

private:
  csOwned<iLight> CreateLight();
  void AddToScene(csWorld *world);
  void RemoveFromScene(csWorld * world);

  void UpdateValues();


  csRef<iLight> m_light;
  iPointLight* m_pointLight;
  iDirectionalLight* m_directionalLight;

  csRef<csGfxLight> m_gfxLight;

  eLightType m_lightType;

  bool m_castShadow;
  float     m_shadowBias;
  csColor4f m_color;
  float     m_range;
};

}

