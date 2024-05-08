
#include "testhandler.hh"

using namespace ce;

BaseTestHandler::BaseTestHandler(const ce::Vector3f &basePosition, float distance, float animationSpeed)
    : ce::EntityState()
    , m_basePosition(basePosition)
    , m_distance(distance)
    , m_animationSpeed(animationSpeed)
{
  SetNeedUpdate(true);
}

BaseTestHandler::~BaseTestHandler()
{

}

void BaseTestHandler::Update(float tpf)
{
  m_animationValue += tpf * m_animationSpeed;
  SpatialState *root = GetRoot();
  if (root)
  {
    Vector3f position = m_basePosition + Vector3f(
        ceCos(m_animationValue) * m_distance,
        0.0f,
        ceSin(m_animationValue*1.3) * m_distance
        );
    root->SetTransform(root->GetTransform().SetTranslation(position));
  }
}


TestHandler01::TestHandler01(const ce::Vector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler02::TestHandler02(const ce::Vector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler03::TestHandler03(const ce::Vector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler04::TestHandler04(const ce::Vector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}
