#include <ceCore/graphics/idevice.hh>
#include <ceCore/java.hh>


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
  auto                       device   = reinterpret_cast<ce::iDevice *>(ref);
  ce::iTexture2D::Descriptor desc {
      (ce::ePixelFormat) pixelFormat,
      (ce::uint16_t) width,
      (ce::uint16_t) height,
      (bool) mipMaps,
      (ce::uint16_t) multiSamples
  };
  ce::iTexture2D             *texture = device->CreateTexture(desc);
  return texture ? texture->GetJObject() : nullptr;
}



}
