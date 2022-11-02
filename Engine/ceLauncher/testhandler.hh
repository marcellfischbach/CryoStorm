
#pragma once

#include <ceCore/entity/entitysystem.hh>


CE_CLASS()
class BaseTestHandler : public CE_SUPER(ce::EntityState)
{
  CE_CLASS_GEN_OBJECT;
protected:
  BaseTestHandler(const ce::Vector3f &basePosition, float distance, float animationSpeed);

public:
  ~BaseTestHandler() override;

  void Update(float tpf) override;

private:
  ce::Vector3f m_basePosition;
  float m_distance;
  float m_animationValue = 0.0f;
  float m_animationSpeed;

};

CE_CLASS()
class TestHandler01 : public CE_SUPER(BaseTestHandler)
{
  CE_CLASS_GEN;
public:
  TestHandler01(const ce::Vector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler01() override = default;
};

CE_CLASS()
class TestHandler02 : public CE_SUPER(BaseTestHandler)
{
  CE_CLASS_GEN;
public:
  TestHandler02(const ce::Vector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler02() override = default;
};


CE_CLASS()
class TestHandler03 : public CE_SUPER(BaseTestHandler)
{
  CE_CLASS_GEN;
public:
  TestHandler03(const ce::Vector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler03() override = default;
};


CE_CLASS()
class TestHandler04 : public CE_SUPER(BaseTestHandler)
{
  CE_CLASS_GEN;
public:
  TestHandler04(const ce::Vector3f &basePosition, float distance, float animationSpeed);
  ~TestHandler04() override = default;
};

