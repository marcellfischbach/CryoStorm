
#include "testhandler.hh"

using namespace cs;

BaseTestHandler::BaseTestHandler(const cs::csVector3f &basePosition, float distance, float animationSpeed)
    : cs::csEntityState()
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
  csRef<csSpatialState> &root = GetRoot();
  if (root)
  {
    csVector3f position = m_basePosition + csVector3f(
        ceCos(m_animationValue) * m_distance,
        0.0f,
        ceSin(m_animationValue*1.3f) * m_distance
    );
    root->SetTransform(root->GetTransform().SetTranslation(position));
  }
}


TestHandler01::TestHandler01(const cs::csVector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler02::TestHandler02(const cs::csVector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler03::TestHandler03(const cs::csVector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}


TestHandler04::TestHandler04(const cs::csVector3f &basePosition, float distance, float animationSpeed)
    : BaseTestHandler(basePosition, distance, animationSpeed)
{

}
