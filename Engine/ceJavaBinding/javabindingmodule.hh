//
// Created by MCEL on 08.05.2024.
//

#pragma once


#include <ceCore/imodule.hh>


namespace ce::java
{

class JavaBindingModule : public ce::iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}
