
#include <ceCore/graphics/material/material.hh>


extern "C"
{

#define CS_MAT(ref) reinterpret_cast<ce::Material *>(ref)


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nGetFillMode(JNIEnv *env, jclass cls, jlong materialRef)
{
  auto material = CS_MAT(materialRef);
  return (jint) material->GetFillMode();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nGetRenderQueue(JNIEnv *env, jclass cls, jlong materialRef)
{
  auto material = CS_MAT(materialRef);
  return (jint) material->GetRenderQueue();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nGetShadingMode(JNIEnv *env, jclass cls, jlong materialRef)
{
  auto material = CS_MAT(materialRef);
  return (jint) material->GetShadingMode();
}

JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nIndexOf(JNIEnv *env,
                                                                      jclass cls,
                                                                      jlong materialRef,
                                                                      jstring attributeName)
{
  auto       material = CS_MAT(materialRef);
  const char *chars   = env->GetStringUTFChars(attributeName, 0);
  jint       res      = (jint) material->IndexOf(chars);
  env->ReleaseStringUTFChars(attributeName, chars);
  return res;
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetFloat(JNIEnv *env,
                                                                       jclass cls,
                                                                       jlong materialRef,
                                                                       jint idx,
                                                                       float value)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, value);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetVec2f(JNIEnv *env,
                                                                       jclass cls,
                                                                       jlong materialRef,
                                                                       jint idx,
                                                                       float x,
                                                                       float y)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, ce::Vector2f(x, y));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetVec3f(JNIEnv *env,
                                                                       jclass cls,
                                                                       jlong materialRef,
                                                                       jint idx,
                                                                       float x,
                                                                       float y,
                                                                       float z)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, ce::Vector3f(x, y, z));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetVec4f(JNIEnv *env,
                                                                       jclass cls,
                                                                       jlong materialRef,
                                                                       jint idx,
                                                                       float x,
                                                                       float y,
                                                                       float z,
                                                                       float w)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, ce::Vector4f(x, y, z, w));
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetColor4f(JNIEnv *env,
                                                                         jclass cls,
                                                                         jlong materialRef,
                                                                         jint idx,
                                                                         float r,
                                                                         float g,
                                                                         float b,
                                                                         float a)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, ce::Color4f(r, g, b, a));
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetInt(JNIEnv *env,
                                                                     jclass cls,
                                                                     jlong materialRef,
                                                                     jint idx,
                                                                     jint value)
{
  auto material = CS_MAT(materialRef);
  material->Set(idx, (int) value);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetMatrix3f(JNIEnv *env,
                                                                          jclass cls,
                                                                          jlong materialRef,
                                                                          jint idx,
                                                                          jfloatArray mArray)
{
  auto   material = CS_MAT(materialRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  material->Set(idx, *reinterpret_cast<ce::Matrix3f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetMatrix4f(JNIEnv *env,
                                                                          jclass cls,
                                                                          jlong materialRef,
                                                                          jint idx,
                                                                          jfloatArray mArray)
{
  auto   material = CS_MAT(materialRef);
  jfloat *m       = env->GetFloatArrayElements(mArray, 0);

  material->Set(idx, *reinterpret_cast<ce::Matrix4f *>(m));

  env->ReleaseFloatArrayElements(mArray, m, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_material_Material_nSetTexture(JNIEnv *env,
                                                                          jclass cls,
                                                                          jlong materialRef,
                                                                          jint idx,
                                                                          jlong textureRef)
{
  auto   material = CS_MAT(materialRef);
  auto texture = reinterpret_cast<ce::iTexture*>(textureRef);

  material->Set(idx, texture);
}

}