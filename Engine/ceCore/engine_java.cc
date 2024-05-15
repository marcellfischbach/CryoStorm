
#include <ceCore/engine.hh>
#include <ceCore/java.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/graphics/idevice.hh>


extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetWorld(JNIEnv *env, jclass cls)
{
  return ce::Engine::Get()->GetWorld()->GetJObject();
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetDevice(JNIEnv *env, jclass cls)
{
  return ce::Engine::Get()->GetDevice()->GetJObject();
}

}

