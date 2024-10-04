
#include <csCore/input/iInputSystem.hh>
#include <csCore/input/iMouse.hh>
#include <csCore/input/iKeyboard.hh>


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
