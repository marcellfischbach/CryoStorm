
#include <csCore/graphics/material/csMaterialInstance.hh>


extern "C"
{

#define CS_MAT(ref) reinterpret_cast<cs::csMaterial*>(ref)
#define CS_INSTANCE(ref) reinterpret_cast<cs::csMaterialInstance*>(ref)


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMaterial(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jlong materialRef)
{
  auto instance = CS_INSTANCE(instanceRef);
  auto material = CS_MAT(materialRef);
  instance->SetMaterial(material);
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetMaterial(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef)
{
  auto             instance  = CS_INSTANCE(instanceRef);
  cs::csMaterial *material = instance->GetMaterial();
  return material ? material->GetJObject() : nullptr;
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetFillMode(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef)
{
  auto instance = CS_INSTANCE(instanceRef);
  return (jint) instance->GetFillMode();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetRenderQueue(JNIEnv *env,
                                                                                     jclass cls,
                                                                                     jlong instanceRef)
{
  auto instance = CS_INSTANCE(instanceRef);
  return (jint) instance->GetRenderQueue();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetShadingMode(JNIEnv *env,
                                                                                     jclass cls,
                                                                                     jlong instanceRef)
{
  auto instance = CS_INSTANCE(instanceRef);
  return (jint) instance->GetShadingMode();
}

JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nIndexOf(JNIEnv *env,
                                                                              jclass cls,
                                                                              jlong instanceRef,
                                                                              jstring attributeName)
{
  auto       instance = CS_INSTANCE(instanceRef);
  const char *chars   = env->GetStringUTFChars(attributeName, 0);
  jint       res      = (jint) instance->IndexOf(chars);
  env->ReleaseStringUTFChars(attributeName, chars);
  return res;
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetFloat(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong instanceRef,
                                                                               jint idx,
                                                                               float value)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, value);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetVec2f(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong instanceRef,
                                                                               jint idx,
                                                                               float x,
                                                                               float y)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, cs::csVector2f(x, y));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetVec3f(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong instanceRef,
                                                                               jint idx,
                                                                               float x,
                                                                               float y,
                                                                               float z)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, cs::csVector3f(x, y, z));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetVec4f(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong instanceRef,
                                                                               jint idx,
                                                                               float x,
                                                                               float y,
                                                                               float z,
                                                                               float w)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, cs::csVector4f(x, y, z, w));
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetColor4f(JNIEnv *env,
                                                                                 jclass cls,
                                                                                 jlong instanceRef,
                                                                                 jint idx,
                                                                                 float r,
                                                                                 float g,
                                                                                 float b,
                                                                                 float a)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, cs::csColor4f(r, g, b, a));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetInt(JNIEnv *env,
                                                                             jclass cls,
                                                                             jlong instanceRef,
                                                                             jint idx,
                                                                             jint value)
{
  auto instance = CS_INSTANCE(instanceRef);
  instance->Set(idx, (int) value);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMatrix3f(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jint idx,
                                                                                  jfloatArray mArray)
{
  auto   instance = CS_INSTANCE(instanceRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  instance->Set(idx, *reinterpret_cast<cs::csMatrix3f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMatrix4f(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jint idx,
                                                                                  jfloatArray mArray)
{
  auto   instance = CS_INSTANCE(instanceRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  instance->Set(idx, *reinterpret_cast<cs::csMatrix4f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetTexture(JNIEnv *env,
                                                                                 jclass cls,
                                                                                 jlong instanceRef,
                                                                                 jint idx,
                                                                                 jlong textureRef)
{
  auto instance = CS_INSTANCE(instanceRef);
  auto texture  = reinterpret_cast<cs::iTexture *>(textureRef);

  instance->Set(idx, texture);
}


}