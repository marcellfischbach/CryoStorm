#include <ceCore/entity/entity.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/world.hh>


extern "C"
{

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_Entity_nSetName(JNIEnv *env, jclass cls, jlong entityRef, jstring name)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);

  const char *string = env->GetStringUTFChars(name, 0);
  entity->SetName(string);
  env->ReleaseStringUTFChars(name, string);
}

JNIEXPORT jstring
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetName(JNIEnv *env, jclass cls, jlong entityRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
  return env->NewStringUTF(entity->GetName().c_str());
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetWorld(JNIEnv *env, jclass cls, jlong entityRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
  cryo::World *pWorld = entity->GetWorld();
  return pWorld ? pWorld->GetJObject() : nullptr;
}


JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nAttachEntity(JNIEnv *env, jclass cls, jlong thisRef, jlong childRef, jlong parentStateRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  auto parent = reinterpret_cast<cryo::SpatialState*>(parentStateRef);
  auto child  = reinterpret_cast<cryo::Entity *>(childRef);

  return entity->Attach(child, parent);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nDetachEntity(JNIEnv *env, jclass cls, jlong thisRef, jlong childRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  auto child  = reinterpret_cast<cryo::Entity *>(childRef);

  return entity->Detach(child);
}



JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetParent(JNIEnv *env, jclass cls, jlong entityRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
  cryo::Entity *pEntity = entity->GetParent();
  return pEntity ? pEntity->GetJObject() : nullptr;
}



JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetNumberOfChildren(JNIEnv *env, jclass cls, jlong entityRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
  return (jint)entity->GetNumberOfChildren();
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetChild(JNIEnv *env, jclass cls, jlong entityRef, jint idx)
{
  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
  cryo::Entity *pEntity = entity->GetChild(idx);
  return pEntity ? pEntity->GetJObject() : nullptr;
}



JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nAttachState(JNIEnv *env, jclass cls, jlong thisRef, jlong stateRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  auto state  = reinterpret_cast<cryo::EntityState *>(stateRef);

  return entity->Attach(state);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nDetachState(JNIEnv *env, jclass cls, jlong thisRef, jlong stateRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  auto state  = reinterpret_cast<cryo::EntityState *>(stateRef);

  return entity->Detach(state);
}



JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_Entity_nSetRoot(JNIEnv *env, jclass cls, jlong thisRef, jlong rootStateRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  auto state  = reinterpret_cast<cryo::SpatialState *>(rootStateRef);

  entity->SetRoot(state);
}



JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetRoot(JNIEnv *env, jclass cls, jlong thisRef)
{
  auto entity = reinterpret_cast<cryo::Entity *>(thisRef);
  cryo::SpatialState *pState = entity->GetRoot();
  pState? pState->GetJObject() : nullptr;
}


}