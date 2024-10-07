//
// Created by MCEL on 08.05.2024.
//

#pragma once


#include <csCore/iModule.hh>


namespace cs::java
{

class csJavaBindingModule : public cs::iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}
