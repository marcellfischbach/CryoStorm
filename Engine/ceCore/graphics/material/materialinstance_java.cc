
#include <ceCore/graphics/material/materialinstance.hh>


extern "C"
{

#define CE_MAT(ref) reinterpret_cast<ce::Material*>(ref)
#define CE_INSTANCE(ref) reinterpret_cast<ce::MaterialInstance*>(ref)


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMaterial(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jlong materialRef)
{
  auto instance = CE_INSTANCE(instanceRef);
  auto material = CE_MAT(materialRef);
  instance->SetMaterial(material);
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetMaterial(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef)
{
  auto         instance  = CE_INSTANCE(instanceRef);
  ce::Material *material = instance->GetMaterial();
  return material ? material->GetJObject() : nullptr;
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetFillMode(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef)
{
  auto instance = CE_INSTANCE(instanceRef);
  return (jint) instance->GetFillMode();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetRenderQueue(JNIEnv *env,
                                                                                     jclass cls,
                                                                                     jlong instanceRef)
{
  auto instance = CE_INSTANCE(instanceRef);
  return (jint) instance->GetRenderQueue();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nGetShadingMode(JNIEnv *env,
                                                                                     jclass cls,
                                                                                     jlong instanceRef)
{
  auto instance = CE_INSTANCE(instanceRef);
  return (jint) instance->GetShadingMode();
}

JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nIndexOf(JNIEnv *env,
                                                                              jclass cls,
                                                                              jlong instanceRef,
                                                                              jstring attributeName)
{
  auto       instance = CE_INSTANCE(instanceRef);
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
  auto instance = CE_INSTANCE(instanceRef);
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
  auto instance = CE_INSTANCE(instanceRef);
  instance->Set(idx, ce::Vector2f(x, y));
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
  auto instance = CE_INSTANCE(instanceRef);
  instance->Set(idx, ce::Vector3f(x, y, z));
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
  auto instance = CE_INSTANCE(instanceRef);
  instance->Set(idx, ce::Vector4f(x, y, z, w));
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
  auto instance = CE_INSTANCE(instanceRef);
  instance->Set(idx, ce::Color4f(r, g, b, a));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetInt(JNIEnv *env,
                                                                             jclass cls,
                                                                             jlong instanceRef,
                                                                             jint idx,
                                                                             jint value)
{
  auto instance = CE_INSTANCE(instanceRef);
  instance->Set(idx, (int) value);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMatrix3f(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jint idx,
                                                                                  jfloatArray mArray)
{
  auto   instance = CE_INSTANCE(instanceRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  instance->Set(idx, *reinterpret_cast<ce::Matrix3f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetMatrix4f(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong instanceRef,
                                                                                  jint idx,
                                                                                  jfloatArray mArray)
{
  auto   instance = CE_INSTANCE(instanceRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  instance->Set(idx, *reinterpret_cast<ce::Matrix4f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_MaterialInstance_nSetTexture(JNIEnv *env,
                                                                                 jclass cls,
                                                                                 jlong instanceRef,
                                                                                 jint idx,
                                                                                 jlong textureRef)
{
  auto instance = CE_INSTANCE(instanceRef);
  auto texture  = reinterpret_cast<ce::iTexture *>(textureRef);

  instance->Set(idx, texture);
}


}