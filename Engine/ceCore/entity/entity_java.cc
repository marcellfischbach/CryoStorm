#include <ceCore/entity/entity.hh>

extern "C"
{

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_Entity_nSetName(JNIEnv *env, jclass cls, jlong entityRef, jstring name)
{
  auto entity = reinterpret_cast<ce::Entity *>(entityRef);

  const char *string = env->GetStringUTFChars(name, 0);
  entity->SetName(string);
  env->ReleaseStringUTFChars(name, string);
}

JNIEXPORT jstring
JNICALL Java_org_crimsonedge_core_entity_Entity_nGetName(JNIEnv *env, jclass cls, jlong entityRef)
{
  auto entity = reinterpret_cast<ce::Entity *>(entityRef);
  return env->NewStringUTF(entity->GetName().c_str());
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nAttachEntity(JNIEnv *env, jclass cls, jlong thisRef, jlong childRef)
{
  auto entity = reinterpret_cast<ce::Entity *>(thisRef);
  auto child  = reinterpret_cast<ce::Entity *>(childRef);

  return entity->Attach(child);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nDetachEntity(JNIEnv *env, jclass cls, jlong thisRef, jlong childRef)
{
  auto entity = reinterpret_cast<ce::Entity *>(thisRef);
  auto child  = reinterpret_cast<ce::Entity *>(childRef);

  return entity->Detach(child);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nAttachState(JNIEnv *env, jclass cls, jlong thisRef, jlong stateRef)
{
  auto entity = reinterpret_cast<ce::Entity *>(thisRef);
  auto state  = reinterpret_cast<ce::EntityState *>(stateRef);

  return entity->Attach(state);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_Entity_nDetachState(JNIEnv *env, jclass cls, jlong thisRef, jlong stateRef)
{
  auto entity = reinterpret_cast<ce::Entity *>(thisRef);
  auto state  = reinterpret_cast<ce::EntityState *>(stateRef);

  return entity->Detach(state);
}

}