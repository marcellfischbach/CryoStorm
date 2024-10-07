#include <csCore/graphics/iTexture2D.hh>
#include <csCore/csJava.hh>


extern "C"
{


JNIEXPORT jboolean JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nIsMultiSampling(JNIEnv *env,
                                                                                          jclass cls,
                                                                                          jlong ref)
{
  auto texture = reinterpret_cast<cs::iTexture2D *>(ref);
  return texture->IsMultiSampling();
}


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nGetSamples(JNIEnv *env,
                                                                                 jclass cls,
                                                                                 jlong ref)
{
  auto texture = reinterpret_cast<cs::iTexture2D *>(ref);
  return texture->GetSamples();
}


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nGetWidth(JNIEnv *env,
                                                                               jclass cls,
                                                                               jlong ref)
{
  auto texture = reinterpret_cast<cs::iTexture2D *>(ref);
  return texture->GetWidth();
}


JNIEXPORT jint JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nGetHeight(JNIEnv *env,
                                                                                jclass cls,
                                                                                jlong ref)
{
  auto texture = reinterpret_cast<cs::iTexture2D *>(ref);
  return texture->GetHeight();
}


JNIEXPORT void JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nData(JNIEnv *env,
                                                                           jclass cls,
                                                                           jlong ref,
                                                                           jint level,
                                                                           jint pixelFormat,
                                                                           jbyteArray dataArray)
{
  auto  texture = reinterpret_cast<cs::iTexture2D *>(ref);
  jbyte *data   = env->GetByteArrayElements(dataArray, 0);
  texture->Data(
      level,
      (cs::ePixelFormat) pixelFormat,
      data
  );
  env->ReleaseByteArrayElements(dataArray, data, 0);
}


JNIEXPORT void JNICALL Java_org_crimsonedge_core_graphics_ITexture2D_nDataExt(JNIEnv *env,
                                                                              jclass cls,
                                                                              jlong ref,
                                                                              jint level,
                                                                              jint x,
                                                                              jint y,
                                                                              jint width,
                                                                              jint height,
                                                                              jint pixelFormat,
                                                                              jbyteArray dataArray)
{
  auto  texture = reinterpret_cast<cs::iTexture2D *>(ref);
  jbyte *data   = env->GetByteArrayElements(dataArray, 0);
  texture->Data(
      level,
      x,
      y,
      width,
      height,
      (cs::ePixelFormat) pixelFormat,
      data
  );
  env->ReleaseByteArrayElements(dataArray, data, 0);
}

}