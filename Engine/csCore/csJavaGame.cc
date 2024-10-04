//
// Created by Marcell on 10.05.2024.
//

#include <csCore/csJavaGame.hh>
#include <csCore/csJava.hh>


namespace cryo
{

csGameJava::csGameJava(jobject gameObject)
: m_gameObject(gameObject)
{

}


bool csGameJava::Initialize(cryo::csEngine *engine)
{
  static jclass cls = csJava::Get()->FindClass("org/crimsonedge/core/IGame");
  if (cls)
  {
    static jmethodID initialize = csJava::Get()->GetMethodID(cls, "initialize", "()Z");
    if (initialize)
    {
      return csJava::Get()->CallBooleanMethod(m_gameObject, initialize);
    }
  }
  return false;
}

}