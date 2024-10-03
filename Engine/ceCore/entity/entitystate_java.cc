#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/entity.hh>


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_EntityState_nSetName(JNIEnv *env, jclass cls, jlong ref, jstring nameString)
{
  const char* name = env->GetStringUTFChars(nameString, 0);

  auto entityState = reinterpret_cast<cryo::EntityState*>(ref);
  entityState->SetName(name);

  env->ReleaseStringUTFChars(nameString, name);
}

JNIEXPORT jstring
JNICALL Java_org_crimsonedge_core_entity_EntityState_nGetName(JNIEnv *env, jclass cls, jlong ref)
{

  auto entityState = reinterpret_cast<cryo::EntityState*>(ref);
  const std::string &name = entityState->GetName();

  return env->NewStringUTF(name.c_str());
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_EntityState_nSetEntity(JNIEnv *env, jclass cls, jlong ref, jlong entityRef)
{
  auto entityState = reinterpret_cast<cryo::EntityState*>(ref);
  auto entity = reinterpret_cast<cryo::Entity*>(entityRef);
  entityState->SetEntity(entity);
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_EntityState_nGetEntity(JNIEnv *env, jclass cls, jlong ref)
{
  auto entityState = reinterpret_cast<cryo::EntityState*>(ref);
  cryo::Entity *pEntity = entityState->GetEntity();
  return pEntity ? pEntity->GetJObject() : nullptr;
}

}
