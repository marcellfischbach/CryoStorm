#include <csCore/graphics/csMesh.hh>
#include <csCore/csJava.hh>


extern "C"
{


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nAddMaterialSlot(JNIEnv *env,
                                                                 jclass cls,
                                                                 jlong ref,
                                                                 jstring name,
                                                                 jlong materialRef)
{
  auto       mesh       = reinterpret_cast<cs::csMesh *>(ref);
  const char *nameChars = env->GetStringUTFChars(name, 0);
  auto       material   = reinterpret_cast<cs::iMaterial *>(materialRef);
  size_t     idx        = mesh->AddMaterialSlot(nameChars, material);
  env->ReleaseStringUTFChars(name, nameChars);
  return (jint) idx;
}


JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nSetDefaultMaterial(JNIEnv *env,
                                                                    jclass cls,
                                                                    jlong ref,
                                                                    jint idx,
                                                                    jlong materialRef)
{
  auto mesh     = reinterpret_cast<cs::csMesh *>(ref);
  auto material = reinterpret_cast<cs::iMaterial *>(materialRef);
  mesh->SetDefaultMaterial(idx, material);
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nGetNumberOfMaterialSlots(JNIEnv *env,
                                                                          jclass cls,
                                                                          jlong ref)
{
  auto mesh = reinterpret_cast<cs::csMesh *>(ref);

  return (jint) mesh->GetNumberOfMaterialSlots();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nIndexOfMaterialSlot(JNIEnv *env,
                                                                     jclass cls,
                                                                     jlong ref,
                                                                     jstring name)
{
  auto       mesh       = reinterpret_cast<cs::csMesh *>(ref);
  const char *nameChars = env->GetStringUTFChars(name, 0);
  size_t     idx        = mesh->IndexOfMaterialSlot(nameChars);
  env->ReleaseStringUTFChars(name, nameChars);
  return (jint) idx;
}

JNIEXPORT void
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nAddSubMesh(JNIEnv *env,
                                                            jclass cls,
                                                            jlong ref,
                                                            jlong meshRef,
                                                            jint materialSlotIdx)
{
  auto mesh       = reinterpret_cast<cs::csMesh *>(ref);
  auto renderMesh = reinterpret_cast<cs::iRenderMesh *>(meshRef);
  mesh->AddSubMesh(renderMesh, materialSlotIdx);
}


}