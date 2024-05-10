//
// Created by Marcell on 10.05.2024.
//

#include <ceJavaBinding/javagame.hh>
#include <ceCore/java.hh>


namespace ce::java
{

JavaGame::JavaGame(jobject gameObject)
: m_gameObject(gameObject)
{

}


bool JavaGame::Initialize(ce::Engine *engine)
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