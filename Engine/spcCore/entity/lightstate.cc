
#include <spcCore/entity/lightstate.hh>
#include <spcCore/entity/world.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/idirectionallight.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/scene/gfxlight.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/objectregistry.hh>

namespace spc
{


LightState::LightState(const std::string& name)
        : SpatialState(name), m_light(nullptr), m_lightType(eLT_Point), m_gfxLight(nullptr)
{

}

LightState::~LightState() noexcept
{
  RemoveFromScene(GetWorld());
  SPC_RELEASE(m_light);
  SPC_RELEASE(m_gfxLight);
}

void LightState::SetType(eLightType type)
{
  if (m_lightType != type)
  {
    World* world = GetWorld();
    RemoveFromScene(world);
    AddToScene(world);
  }
}

void LightState::OnAttachedToWorld(World* world)
{
  AddToScene(world);
}


void LightState::OnDetachedFromWorld(World* world)
{
  RemoveFromScene(world);
}


void LightState::TransformationUpdated()
{
  if (m_pointLight)
  {
    const Matrix4f& mat = GetGlobalMatrix();

    m_pointLight->SetPosition(mat.GetTranslation());
  }
  if (m_directionalLight)
  {
    const Matrix4f& mat = GetGlobalMatrix();
    m_directionalLight->SetDirection(mat.GetZAxis());
  }
}

eLightType LightState::GetType() const
{
  return m_lightType;
}


iLight* LightState::CreateLight()
{
  iDevice* device = ObjectRegistry::Get<iDevice>();
  switch (m_lightType)
  {
    case eLT_Point: return device->CreatePointLight();
    case eLT_Directional: return device->CreateDirectionalLight();
  }
  return nullptr;
}

void LightState::UpdateValues()
{
  if (m_light)
  {
    m_light->SetCastShadow(m_castShadow);
    m_light->SetColor(m_color);
    m_light->SetChangeMode(eLCM_Dynamic);
    if (m_pointLight)
    {
      m_pointLight->SetRange(m_range);
    }

    TransformationUpdated();
  }
}

void LightState::AddToScene(World* world)
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
    GfxScene* scene = world->GetScene();
    if (m_gfxLight)
    {
      m_gfxLight->Release();
    }
    m_gfxLight = new GfxLight();
    m_gfxLight->SetStatic(IsStatic());
    m_gfxLight->SetLight(m_light);

    scene->Add(m_gfxLight);
  }
}


void LightState::RemoveFromScene(World* world)
{
  if (m_gfxLight && world)
  {
    world->GetScene()->Remove(m_gfxLight);
  }

  SPC_RELEASE(m_light);
  SPC_RELEASE(m_directionalLight);
  m_light = nullptr;
  m_pointLight = nullptr;
  m_directionalLight = nullptr;
}

void LightState::SetColor(const Color4f& color)
{
  m_color = color;
  UpdateValues();
}

const Color4f& LightState::GetColor() const
{
  return m_color;
}

void LightState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  UpdateValues();
}

bool LightState::IsCastShadow() const
{
  return m_castShadow;
}

void LightState::SetRange(float range)
{
  m_range = range;
  UpdateValues();
}

float LightState::GetRange() const
{
  return m_range;
}


}