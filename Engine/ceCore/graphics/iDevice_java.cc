#include <ceCore/graphics/iDevice.hh>
#include <ceCore/csJava.hh>


extern "C"
{


JNIEXPORT jobject JNICALL Java_org_crimsonedge_core_graphics_IDevice_nCreateTexture2D(JNIEnv *env,
                                                                                      jclass cls,
                                                                                      jlong ref,
                                                                                      jint pixelFormat,
                                                                                      jint width,
                                                                                      jint height,
                                                                                      jboolean mipMaps,
                                                                                      jint multiSamples)
{
  auto                       device   = reinterpret_cast<cryo::iDevice *>(ref);
  cryo::iTexture2D::Descriptor desc {
      (cryo::ePixelFormat) pixelFormat,
      (cryo::uint16_t) width,
      (cryo::uint16_t) height,
      (bool) mipMaps,
      (cryo::uint16_t) multiSamples
  };
  cryo::iTexture2D             *texture = device->CreateTexture(desc);
  return texture ? texture->GetJObject() : nullptr;
}



}
