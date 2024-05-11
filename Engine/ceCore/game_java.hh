//
// Created by Marcell on 10.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/igame.hh>
#include <jni.h>


namespace ce
{

class CE_CORE_API GameJava : public iGame
{
public:
  GameJava(jobject gameObject);

  bool Initialize (ce::Engine* engine) override;

private:
  jobject  m_gameObject;
};


}