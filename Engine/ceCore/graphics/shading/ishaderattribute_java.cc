#include <ceCore/graphics/shading/ishaderattribute.hh>


extern "C"
{


JNIEXPORT jstring
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nGetName(JNIEnv *env,
                                                                             jclass cls,
                                                                             jlong ref)
{
  auto              attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  const std::string &name     = attribute->GetName();
  return env->NewStringUTF(name.c_str());
}


JNIEXPORT jboolean
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nIsValid(JNIEnv *env,
                                                                             jclass cls,
                                                                             jlong ref)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  return attribute->IsValid();
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nSetArrayIndex(JNIEnv *env,
                                                                             jclass cls,
                                                                             jlong ref,
                                                                             jint index)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->SetArrayIndex((cryo::Size)index);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindFloat(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref,
                                                                                   jfloat v)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind(v);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindFloat2(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong ref,
                                                                               jfloat x,
                                                                               jfloat y)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind(x, y);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindFloat3(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref,
                                                                                jfloat x,
                                                                                jfloat y,
                                                                                jfloat z)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind(x, y, z);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindFloat4(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref,
                                                                                jfloat x,
                                                                                jfloat y,
                                                                                jfloat z,
                                                                                jfloat w)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind(x, y, z, w);
}



JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindInt(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong ref,
                                                                               jint v)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind((int)v);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindInt2(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref,
                                                                                jint x,
                                                                                jint y)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind((int)x, (int)y);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindInt3(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref,
                                                                                jint x,
                                                                                jint y,
                                                                                jint z)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind((int)x, (int)y, (int)z);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindInt4(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref,
                                                                                jint x,
                                                                                jint y,
                                                                                jint z,
                                                                                jint w)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  attribute->Bind((int)x, (int)y, (int)z, (int)w);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindMatrix3f(JNIEnv *env,
                                                                              jclass cls,
                                                                              jlong ref,
                                                                              jfloatArray floatArray)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  jfloat *m      = env->GetFloatArrayElements(floatArray, 0);
  attribute->Bind(*reinterpret_cast<const cryo::Matrix3f*>(m));
  env->ReleaseFloatArrayElements(floatArray, m, 0);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_shading_IShaderAttribute_nBindMatrix4f(JNIEnv *env,
                                                                                  jclass cls,
                                                                                  jlong ref,
                                                                                  jfloatArray floatArray)
{
  auto attribute = reinterpret_cast<cryo::iShaderAttribute *>(ref);
  jfloat *m      = env->GetFloatArrayElements(floatArray, 0);
  attribute->Bind(*reinterpret_cast<const cryo::Matrix4f*>(m));
  env->ReleaseFloatArrayElements(floatArray, m, 0);
}

}
