
#include <ceCore/entity/spatialstate.hh>

#define CE_SPATIAL_STATE(ref) reinterpret_cast<ce::SpatialState*>(ref)

extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nSetStatic(JNIEnv *env, jclass cls, jlong ref, jboolean _static)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  spatialState->SetStatic(_static);
  
}

JNIEXPORT jboolean 
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nIsStatic(JNIEnv *env, jclass cls, jlong ref)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  return spatialState->IsStatic();
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nAttach(JNIEnv *env, jclass cls, jlong ref, jlong childRef)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  auto child = CE_SPATIAL_STATE(childRef);
  return spatialState->Attach(child);
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nDetachSelf(JNIEnv *env, jclass cls, jlong ref)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  return spatialState->DetachSelf();
}

JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nDetach(JNIEnv *env, jclass cls, jlong ref, jlong childRef)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  auto child = CE_SPATIAL_STATE(childRef);
  return spatialState->Detach(child);
  
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nGetParent(JNIEnv *env, jclass cls, jlong ref)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  ce::SpatialState *pState = spatialState->GetParent();
  return pState ? pState->CreateJObject() : nullptr;
}

JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nGetNumberOfChildren(JNIEnv *env, jclass cls, jlong ref)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  return spatialState->GetNumberOfChildren();
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_entity_SpatialState_nGetChild(JNIEnv *env, jclass cls, jlong ref, jint idx)
{
  auto spatialState = CE_SPATIAL_STATE(ref);
  ce::SpatialState *pState = spatialState->GetChild(idx);
  return pState ? pState->GetJObject() : nullptr;
  
}

}