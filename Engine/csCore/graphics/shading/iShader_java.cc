
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>


extern "C"
{


JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_shading_IShader_nRegisterAttribute(JNIEnv *env,
                                                                              jclass cls,
                                                                              jlong ref,
                                                                              jstring attributeNameString)
{
  cs::iShader *shader        = reinterpret_cast<cs::iShader *>(ref);
  const char  *attributeName = env->GetStringUTFChars(attributeNameString, 0);
  cs::Size    idx            = shader->RegisterAttribute(attributeName);
  env->ReleaseStringUTFChars(attributeNameString, attributeName);
  return (jint) idx;
}

JNIEXPORT jint
JNICALL Java_org_crimsonedge_core_graphics_shading_IShader_nGetAttributeId(JNIEnv *env,
                                                                           jclass cls,
                                                                           jlong ref,
                                                                           jstring attributeNameString)
{
  cs::iShader *shader        = reinterpret_cast<cs::iShader *>(ref);
  const char  *attributeName = env->GetStringUTFChars(attributeNameString, 0);
  cs::Size    idx            = shader->GetAttributeId(attributeName);
  env->ReleaseStringUTFChars(attributeNameString, attributeName);
  return (jint) idx;
}

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_shading_IShader_nGetShaderAttributeName(JNIEnv *env,
                                                                                   jclass cls,
                                                                                   jlong ref,
                                                                                   jstring attributeNameString)
{
  cs::iShader          *shader        = reinterpret_cast<cs::iShader *>(ref);
  const char           *attributeName = env->GetStringUTFChars(attributeNameString, 0);
  cs::iShaderAttribute *pAttribute    = shader->GetShaderAttribute(attributeName);
  env->ReleaseStringUTFChars(attributeNameString, attributeName);
  return pAttribute ? pAttribute->GetJObject() : nullptr;
}


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_graphics_shading_IShader_nGetShaderAttributeId(JNIEnv *env,
                                                                                 jclass cls,
                                                                                 jlong ref,
                                                                                 jint id)
{
  cs::iShader          *shader     = reinterpret_cast<cs::iShader *>(ref);
  cs::iShaderAttribute *pAttribute = shader->GetShaderAttribute(id);
  return pAttribute ? pAttribute->GetJObject() : nullptr;
}

}
