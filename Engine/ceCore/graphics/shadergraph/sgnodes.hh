
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API SGConstVec2 : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  SGConstVec2();
  ~SGConstVec2() override = default;

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGConstColor3 : public CE_SUPER(SGNode)
{
  CE_CLASS_GEN;
public:
  SGConstColor3();
  ~SGConstColor3() override = default;

  void CalcIOTypes() override;
};


CE_CLASS()
class CE_CORE_API SGAdd : public CE_SUPER(SGNode)
{
CE_CLASS_GEN;
public:
  SGAdd();
  ~SGAdd() override = default;

  void CalcIOTypes() override;

};


} // ce
