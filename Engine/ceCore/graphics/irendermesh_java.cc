
#include <ceCore/graphics/irendermesh.hh>


extern "C"
{


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetVertices2f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 2;
  auto values = (ce::Vector2f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector2f> vector;
  vector.reserve(length);
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetVertices(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetVertices3f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 3;
  auto values = (ce::Vector3f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector3f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetVertices(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetVertices4f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 4;
  auto values = (ce::Vector4f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector4f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetVertices(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetNormals3f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 3;
  auto values = (ce::Vector3f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector3f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetNormals(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetColors4f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 4;
  auto values = (ce::Color4f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Color4f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetColors(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetTangents3f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 3;
  auto values = (ce::Vector3f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector3f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetTangents(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetUV02f(JNIEnv *env, jclass cls, jlong ref, jfloatArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer) / 2;
  auto values = (ce::Vector2f*)env->GetFloatArrayElements(buffer, 0);
  std::vector<ce::Vector2f> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetUV0(vector);
  env->ReleaseFloatArrayElements(buffer, (jfloat *)values, 0);
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nSetIndices(JNIEnv *env, jclass cls, jlong ref, jintArray buffer)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator*>(ref);
  jsize length   = env->GetArrayLength(buffer);
  auto values = env->GetIntArrayElements(buffer, 0);
  std::vector<uint32_t> vector;
  for (size_t i=0; i<length; i++) {
    vector.push_back(values[i]);
  }
  generator->SetIndices(vector);
  env->ReleaseIntArrayElements(buffer, values, 0);
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGenerator_nGenerate(JNIEnv *env, jclass cls, jlong ref)
{
  auto generator = reinterpret_cast<ce::iRenderMeshGenerator *>(ref);
  ce::iRenderMesh *pMesh = generator->Generate();
  return pMesh ? pMesh->GetJObject() : nullptr;
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGeneratorFactory_nCreate(JNIEnv *env, jclass cls, jlong ref)
{
  auto                     generatorFactory = reinterpret_cast<ce::iRenderMeshGeneratorFactory *>(ref);
  ce::iRenderMeshGenerator *pGenerator      = generatorFactory ? generatorFactory->Create() : nullptr;
  return pGenerator ? pGenerator->CreateJObject() : nullptr;
}

}

