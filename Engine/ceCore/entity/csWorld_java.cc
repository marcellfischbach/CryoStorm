#include <ceCore/entity/csWorld.hh>
#include <ceCore/java.hh>


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_World_nAttach(JNIEnv *env, jclass cls, jlong worldRef, jlong entityRef)
{
  auto world  = reinterpret_cast<cryo::csWorld *>(worldRef);
  auto entity = reinterpret_cast<cryo::csEntity *>(entityRef);


  if (world && entity)
  {
    world->Attach(entity);
  }
}

}

