
#include <ceCore/input/iinputsystem.hh>
#include <ceCore/input/imouse.hh>
#include <ceCore/input/ikeyboard.hh>


extern "C"
{



JNIEXPORT void
JNICALL Java_org_crimsonedge_core_input_IInputSystem_nGetName(JNIEnv *env, jclass cls, jlong entityRef, jstring name)
{
//  auto entity = reinterpret_cast<ce::Entity *>(entityRef);
//
//  const char *string = env->GetStringUTFChars(name, 0);
//  entity->SetName(string);
//  env->ReleaseStringUTFChars(name, string);
}

}
