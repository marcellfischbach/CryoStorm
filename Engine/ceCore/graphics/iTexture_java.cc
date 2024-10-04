#include <ceCore/graphics/iTexture.hh>
#include <ceCore/csJava.hh>


extern "C"
{


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture_nGetTextureType(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref)
{
  auto texture = reinterpret_cast<cryo::iTexture*>(ref);
  return texture->GetType();
}


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture_nGetPixelFormat(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref)
{
  auto texture = reinterpret_cast<cryo::iTexture*>(ref);
  return texture->GetFormat();
}


}