//
// Created by Marcell on 10.05.2024.
//

#include <ceCore/game_java.hh>
#include <ceCore/java.hh>


namespace ce
{

GameJava::GameJava(jobject gameObject)
: m_gameObject(gameObject)
{

}


bool GameJava::Initialize(ce::Engine *engine)
{
  static jclass cls = Java::Get()->FindClass("org/crimsonedge/core/IGame");
  printf ("cls: %p\n", cls);
  if (cls)
  {
    static jmethodID initialize = Java::Get()->GetMethodID(cls, "initialize", "()Z");
    if (initialize)
    {
      return Java::Get()->CallBooleanMethod(m_gameObject, initialize);
    }
  }
  return false;
}

}