//
// Created by MCEL on 08.05.2024.
//

#pragma once


#include <csCore/iModule.hh>


namespace cs::java
{

CS_CLASS()
class csJavaBindingModule : public CS_SUPER(cs::iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}
