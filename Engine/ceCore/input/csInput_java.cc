
#include <ceCore/input/iInputSystem.hh>
#include <ceCore/input/iMouse.hh>
#include <ceCore/input/iKeyboard.hh>


extern "C"
{



JNIEXPORT void
JNICALL Java_org_crimsonedge_core_input_IInputSystem_nGetName(JNIEnv *env, jclass cls, jlong entityRef, jstring name)
{
//  auto entity = reinterpret_cast<cryo::Entity *>(entityRef);
//
//  const char *string = env->GetStringUTFChars(name, 0);
//  entity->SetName(string);
//  env->ReleaseStringUTFChars(name, string);
}

}
