//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <jni.h>

namespace ce
{


class CE_CORE_API Java
{
public:
  static void Set(JNIEnv *env);
  static JNIEnv  *Get();

private:
  Java () = default;

  static JNIEnv *s_env;
};

}
