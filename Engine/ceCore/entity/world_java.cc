#include <ceCore/entity/world.hh>
#include <ceCore/java.hh>


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_World_nAttach(JNIEnv *env, jclass cls, jlong worldRef, jlong entityRef)
{
  auto world  = reinterpret_cast<ce::World *>(worldRef);
  auto entity = reinterpret_cast<ce::Entity *>(entityRef);


  if (world && entity)
  {
    world->Attach(entity);
  }
}

}

