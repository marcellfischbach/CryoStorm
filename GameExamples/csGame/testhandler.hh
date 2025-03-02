
#pragma once

#include <csCore/entity/csEntitySystem.hh>


CS_CLASS()
class BaseTestHandler : public CS_SUPER(cs::csEntityState)
{
  CS_CLASS_GEN_OBJECT;
protected:
  BaseTestHandler(const cs::csVector3f &basePosition, float distance, float animationSpeed);

public:
  ~BaseTestHandler() override;

  void Update(float tpf) override;

private:
  cs::csVector3f m_basePosition;
  float            m_distance;
  float m_animationValue = 0.0f;
  float m_animationSpeed;

};

CS_CLASS()
class TestHandler01 : public CS_SUPER(BaseTestHandler)
{
  CS_CLASS_GEN;
public:
  TestHandler01(const cs::csVector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler01() override = default;
};

CS_CLASS()
class TestHandler02 : public CS_SUPER(BaseTestHandler)
{
  CS_CLASS_GEN;
public:
  TestHandler02(const cs::csVector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler02() override = default;
};


CS_CLASS()
class TestHandler03 : public CS_SUPER(BaseTestHandler)
{
  CS_CLASS_GEN;
public:
  TestHandler03(const cs::csVector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler03() override = default;
};


CS_CLASS()
class TestHandler04 : public CS_SUPER(BaseTestHandler)
{
  CS_CLASS_GEN;
public:
  TestHandler04(const cs::csVector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler04() override = default;
};

