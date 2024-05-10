//
// Created by Marcell on 10.05.2024.
//

#pragma once

#include <ceJavaBinding/javaexport.hh>
#include <ceCore/igame.hh>
#include <jni.h>


namespace ce::java
{



class CE_JAVA_API JavaGame : public iGame
{
public:
  JavaGame (jobject gameObject);

  bool Initialize (ce::Engine* engine) override;

private:
  jobject  m_gameObject;
};


}