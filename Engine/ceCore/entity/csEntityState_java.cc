#include <ceCore/entity/csEntityState.hh>
#include "csEntity.hh"


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_EntityState_nSetName(JNIEnv *env, jclass cls, jlong ref, jstring nameString)
{
  const char* name = env->GetStringUTFChars(nameString, 0);

  auto entityState = reinterpret_cast<cryo::csEntityState*>(ref);
  entityState->SetName(name);

  env->ReleaseStringUTFChars(nameString, name);
}

JNIEXPORT jstring
JNICALL Java_org_crimsonedge_core_entity_EntityState_nGetName(JNIEnv *env, jclass cls, jlong ref)
{

  auto entityState = reinterpret_cast<cryo::csEntityState*>(ref);
  const std::string &name = entityState->GetName();

  return env->NewStringUTF(name.c_str());
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_EntityState_nSetEntity(JNIEnv *env, jclass cls, jlong ref, jlong entityRef)
{
  auto entityState = reinterpret_cast<cryo::csEntityState*>(ref);
  auto entity = reinterpret_cast<cryo::csEntity*>(entityRef);
  entityState->SetEntity(entity);
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_EntityState_nGetEntity(JNIEnv *env, jclass cls, jlong ref)
{
  auto           entityState = reinterpret_cast<cryo::csEntityState*>(ref);
  cryo::csEntity *pEntity    = entityState->GetEntity();
  return pEntity ? pEntity->GetJObject() : nullptr;
}

}
