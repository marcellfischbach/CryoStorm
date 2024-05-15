
#include <ceCore/graphics/irendermesh.hh>


extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_IRenderMeshGeneratorFactory_nCreate(JNIEnv *env, jclass cls, jlong ref)
{
  auto                     generatorFactory = reinterpret_cast<ce::iRenderMeshGeneratorFactory *>(ref);
  ce::iRenderMeshGenerator *pGenerator      = generatorFactory ? generatorFactory->Create() : nullptr;
  return pGenerator ? pGenerator->CreateJObject() : nullptr;
}

}

