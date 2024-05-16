#include <ceCore/graphics/mesh.hh>
#include <ceCore/java.hh>


extern "C"
{


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nAddMaterialSlot(JNIEnv *env,
                                                                 jclass cls,
                                                                 jlong ref,
                                                                 jstring name,
                                                                 jlong materialRef)
{
  auto       mesh       = reinterpret_cast<ce::Mesh *>(ref);
  const char *nameChars = env->GetStringUTFChars(name, 0);
  auto       material   = reinterpret_cast<ce::iMaterial *>(materialRef);
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
  auto mesh     = reinterpret_cast<ce::Mesh *>(ref);
  auto material = reinterpret_cast<ce::iMaterial *>(materialRef);
  mesh->SetDefaultMaterial(idx, material);
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nGetNumberOfMaterialSlots(JNIEnv *env,
                                                                          jclass cls,
                                                                          jlong ref)
{
  auto mesh = reinterpret_cast<ce::Mesh *>(ref);

  return (jint) mesh->GetNumberOfMaterialSlots();
}


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_Mesh_nIndexOfMaterialSlot(JNIEnv *env,
                                                                     jclass cls,
                                                                     jlong ref,
                                                                     jstring name)
{
  auto       mesh       = reinterpret_cast<ce::Mesh *>(ref);
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
  auto mesh       = reinterpret_cast<ce::Mesh *>(ref);
  auto renderMesh = reinterpret_cast<ce::iRenderMesh *>(meshRef);
  mesh->AddSubMesh(renderMesh, materialSlotIdx);
}


}