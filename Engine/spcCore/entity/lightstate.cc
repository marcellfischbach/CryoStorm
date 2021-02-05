
#include <spcCore/entity/lightstate.hh>
#include <spcCore/entity/world.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/idirectionallight.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/objectregistry.hh>

namespace spc
{


LightState::LightState(const std::string &name)
        : SpatialState(name), m_light(nullptr), m_lightType(eLT_Point)
{

}

LightState::~LightState() noexcept
{

}

void LightState::SetType(eLightType type)
{
  if (m_lightType != type)
  {
    World* world = GetWorld();
    if (m_light && world)
    {
      OnDetachedFromWorld(world);
      OnAttachedToWorld(world);
    }
  }
}

void LightState::OnAttachedToWorld(World* world)
{
  GfxScene* scene = world->GetScene();
  if (scene)
  {
    if (!m_light)
    {
      m_light = CreateLight();
      m_pointLight = m_light->Query<iPointLight>();
      m_directionalLight = m_light->Query<iDirectionalLight>();
      UpdateValues();
    }
    scene->Add(m_light);
  }
}

void LightState::OnDetachedFromWorld(World* world)
{
  GfxScene* scene = world->GetScene();
  if (m_light && scene)
  {
    scene->Remove(m_light);
    m_light->Release();
    m_light = nullptr;
    m_pointLight = nullptr;
    m_directionalLight = nullptr;
  }
}

void LightState::TransformationUpdated()
{
  if (m_pointLight)
  {
    const Matrix4f &mat = GetGlobalMatrix();

    m_pointLight->SetPosition(mat.GetTranslation());
  }
  if (m_directionalLight)
  {
    const Matrix4f &mat = GetGlobalMatrix();
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
  if (m_light && world)
  {
    GfxScene* scene = world->GetScene();
    scene->Add(m_light);
  }
}


void LightState::RemoveFromScene(World* world)
{
  if (m_light && world)
  {
    GfxScene* scene = world->GetScene();
    scene->Remove(m_light);
  }
}

void LightState::SetColor(const Color4f& color)
{
  m_color = color;
  UpdateValues();
}

const Color4f&LightState::GetColor() const
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