//
// Created by MCEL on 08.05.2024.
//

#include <ceCore/java.hh>

namespace ce
{

JNIEnv *Java::s_env = nullptr;

void Java::Set(JNIEnv *env)
{
  s_env = env;
}

JNIEnv *Java::Get()
{
  return s_env;
}

}