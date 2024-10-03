//
// Created by Marcell on 10.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/igame.hh>
#include <jni.h>


namespace cryo
{

class CS_CORE_API GameJava : public iGame
{
public:
  GameJava(jobject gameObject);

  bool Initialize (cryo::Engine* engine) override;

private:
  jobject  m_gameObject;
};


}