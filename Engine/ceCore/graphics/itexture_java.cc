#include <ceCore/graphics/itexture.hh>
#include <ceCore/java.hh>


extern "C"
{


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture_nGetTextureType(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref)
{
  auto texture = reinterpret_cast<ce::iTexture*>(ref);
  return texture->GetType();
}


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture_nGetPixelFormat(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref)
{
  auto texture = reinterpret_cast<ce::iTexture*>(ref);
  return texture->GetFormat();
}


}