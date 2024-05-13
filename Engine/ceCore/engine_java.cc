
#include <ceCore/engine.hh>
#include <ceCore/java.hh>
#include <ceCore/entity/world.hh>


extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_Engine_nGetWorld(JNIEnv *env, jclass cls)
{
  return ce::Engine::Get()->GetWorld()->GetJObject();
}

}

