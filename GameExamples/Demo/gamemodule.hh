
#pragma once

#include <ceCore/imodule.hh>



class GameModule : public ce::iModule
{
public:
  bool Register(int argc, char** argv, ce::Engine* engine) override;
  bool Initialize(int argc, char** argv, ce::Engine* engine) override;
};


CE_DECLARE_LIBRARY(ceGame);