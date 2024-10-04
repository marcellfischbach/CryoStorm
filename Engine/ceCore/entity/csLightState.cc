
#include <ceCore/entity/csLightState.hh>
#include <ceCore/entity/csWorld.hh>
#include <ceCore/graphics/iLight.hh>
#include <ceCore/graphics/iPointLight.hh>
#include <ceCore/graphics/iDirectionalLight.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/scene/csGfxLight.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>
#include <ceCore/csObjectRegistry.hh>

namespace cryo
{


csLightState::csLightState(const std::string& name)
  : csSpatialState(name)
  , m_light(nullptr)
  , m_directionalLight(nullptr)
  , m_pointLight(nullptr)
  , m_gfxLight(nullptr)
  , m_lightType(eLT_Point)
  , m_castShadow(false)
  , m_shadowBias(0.001f)
  , m_color(1.0f, 1.0f, 1.0f, 1.0f)
  , m_range (100.0f)
{
  CS_CLASS_GEN_CONSTR;
}

csLightState::~csLightState() noexcept
{
  RemoveFromScene(GetWorld());
  CS_RELEASE(m_light);
  CS_RELEASE(m_gfxLight);
}

void csLightState::SetType(eLightType type)
{
  if (m_lightType != type)
  {
    csWorld * world = GetWorld();
    RemoveFromScene(world);
    m_lightType = type;
    AddToScene(world);
  }
}

void csLightState::OnAttachedToWorld(csWorld* world)
{
  AddToScene(world);
}


void csLightState::OnDetachedFromWorld(csWorld* world)
{
  RemoveFromScene(world);
}


void csLightState::TransformationUpdatedPreChildren()
{
  if (m_pointLight)
  {
    const csMatrix4f & mat = GetGlobalMatrix();

    m_pointLight->SetPosition(mat.GetTranslation());
  }
  if (m_directionalLight)
  {
    const csMatrix4f & mat = GetGlobalMatrix();
    m_directionalLight->SetDirection(mat.GetZAxis());
  }
}

eLightType csLightState::GetType() const
{
  return m_lightType;
}


iLight* csLightState::CreateLight()
{
  iDevice* device = csObjectRegistry::Get<iDevice>();
  switch (m_lightType)
  {
  case eLT_Point: return device->CreatePointLight();
  case eLT_Directional: return device->CreateDirectionalLight();
  }
  return nullptr;
}

void csLightState::UpdateValues()
{
  if (m_light)
  {
    m_light->SetCastShadow(m_castShadow);
    m_light->SetShadowMapBias(m_shadowBias);
    m_light->SetColor(m_color);
    m_light->SetChangeMode(eLCM_Dynamic);

    if (m_pointLight)
    {
      m_pointLight->SetRange(m_range);
    }

    TransformationUpdatedPreChildren();
  }
}

void csLightState::AddToScene(csWorld* world)
{
  if (!m_light)
  {
    m_light = CreateLight();
    m_pointLight = m_light->Query<iPointLight>();
    m_directionalLight = m_light->Query<iDirectionalLight>();
    UpdateValues();
  }
  if (world)
  {
    iGfxScene * scene = world->GetScene();
    if (m_gfxLight)
    {
      m_gfxLight->Release();
    }
    m_gfxLight = new csGfxLight();
    m_gfxLight->SetStatic(IsStatic());
    m_gfxLight->SetLight(m_light);

    scene->Add(m_gfxLight);
  }
}


void csLightState::RemoveFromScene(csWorld* world)
{
  if (m_gfxLight && world)
  {
    world->GetScene()->Remove(m_gfxLight);
  }

  CS_RELEASE(m_light);
  CS_RELEASE(m_gfxLight);
  m_gfxLight = nullptr;
  m_light = nullptr;
  m_pointLight = nullptr;
  m_directionalLight = nullptr;
}

void csLightState::SetColor(const csColor4f& color)
{
  m_color = color;
  UpdateValues();
}

const csColor4f& csLightState::GetColor() const
{
  return m_color;
}

void csLightState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  UpdateValues();
}

bool csLightState::IsCastShadow() const
{
  return m_castShadow;
}

void csLightState::SetShadowMapBias(float bias)
{
  m_shadowBias = bias;
  UpdateValues();
}

float csLightState::GetShadowMapBias() const
{
  return m_shadowBias;
}

void csLightState::SetRange(float range)
{
  m_range = range;
  UpdateValues();
}

float csLightState::GetRange() const
{
  return m_range;
}




}