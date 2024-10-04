
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/graphics/csMesh.hh>


#define CS_STATIC_MESH(ref) reinterpret_cast<cryo::csStaticMeshState*>(ref)

extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nSetMesh(JNIEnv *env, jclass cls, jlong ref, jlong meshRef)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  auto mesh            = reinterpret_cast<cryo::csMesh *>(meshRef);
  staticMeshState->SetMesh(mesh);
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nGetMesh(JNIEnv *env, jclass cls, jlong ref)
{
  auto         staticMeshState = CS_STATIC_MESH(ref);
  cryo::csMesh *pMesh          = staticMeshState->GetMesh();
  return pMesh ? pMesh->GetJObject() : nullptr;
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nSetMaterial(JNIEnv *env,
                                                                      jclass cls,
                                                                      jlong ref,
                                                                      jint idx,
                                                                      jlong materialRef)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  auto material        = reinterpret_cast<cryo::iMaterial *>(materialRef);
  staticMeshState->SetMaterial(idx, material);
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nGetMaterial(JNIEnv *env, jclass cls, jlong ref, jint idx)
{
  auto          staticMeshState = CS_STATIC_MESH(ref);
  cryo::iMaterial *pMaterial      = staticMeshState->GetMaterial(idx);
  return pMaterial ? pMaterial->GetJObject() : nullptr;
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nSetReceiveShadow(JNIEnv *env,
                                                                           jclass cls,
                                                                           jlong ref,
                                                                           jboolean receiveShadow)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  staticMeshState->SetReceiveShadow(receiveShadow);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nIsReceiveShadow(JNIEnv *env, jclass cls, jlong ref)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  return staticMeshState->IsReceiveShadow();
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nSetCastShadow(JNIEnv *env,
                                                                        jclass cls,
                                                                        jlong ref,
                                                                        jboolean castShadow)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  staticMeshState->SetCastShadow(castShadow);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_StaticMeshState_nIsCastShadow(JNIEnv *env, jclass cls, jlong ref)
{
  auto staticMeshState = CS_STATIC_MESH(ref);
  return staticMeshState->IsCastShadow();
}


}