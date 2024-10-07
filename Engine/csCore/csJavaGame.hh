//
// Created by Marcell on 10.05.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/iGame.hh>
#include <jni.h>


namespace cs
{

class CS_CORE_API csGameJava : public iGame
{
public:
  csGameJava(jobject gameObject);

  bool Initialize(cs::csEngine* engine) override;

private:
  jobject  m_gameObject;
};


}