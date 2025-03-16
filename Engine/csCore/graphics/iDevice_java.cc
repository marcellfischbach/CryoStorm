#include <csCore/graphics/iDevice.hh>
#include <csCore/csJava.hh>


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
  auto                       device   = reinterpret_cast<cs::iDevice *>(ref);
  cs::iTexture2D::Descriptor desc {
      (cs::ePixelFormat) pixelFormat,
      (cs::uint16_t) width,
      (cs::uint16_t) height,
      (bool) mipMaps,
      (cs::uint16_t) multiSamples
  };
  cs::csOwned<cs::iTexture2D> texture = device->CreateTexture(desc);
  jobject  result = texture ? texture->GetJObject() : nullptr;
  if (result)
  {
    texture->AddRef();
  }
  return result;
}



}
