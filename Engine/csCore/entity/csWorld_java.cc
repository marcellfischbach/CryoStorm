#include <csCore/entity/csWorld.hh>
#include <csCore/csJava.hh>


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_World_nAttach(JNIEnv *env, jclass cls, jlong worldRef, jlong entityRef)
{
  auto world  = reinterpret_cast<cs::csWorld *>(worldRef);
  auto entity = reinterpret_cast<cs::csEntity *>(entityRef);


  if (world && entity)
  {
    world->Attach(entity);
  }
}

}

