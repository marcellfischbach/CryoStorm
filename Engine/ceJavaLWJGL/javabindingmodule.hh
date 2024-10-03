//
// Created by MCEL on 08.05.2024.
//

#pragma once


#include <ceCore/imodule.hh>


namespace cryo::java
{

class JavaBindingModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}
