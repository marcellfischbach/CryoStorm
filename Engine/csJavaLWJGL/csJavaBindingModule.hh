//
// Created by MCEL on 08.05.2024.
//

#pragma once


#include <csCore/iModule.hh>


namespace cryo::java
{

class csJavaBindingModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}
