
#include <csCore/entity/csJavaState.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>
#include <csCore/entity/csWorld.hh>

#define THIS_CLASS_NAME  "org/cryo/core/entity/JavaState"

namespace cs
{


csJavaState::csJavaState()
: csEntityState()
{
  SetNeedUpdate(true);
}

void csJavaState::OnAttachedToWorld(cs::csWorld *world)
{
  static csJavaCallVoid1<jobject> onAttachedToWorld (csJava::Get(), this, THIS_CLASS_NAME, "onAttachedToWorld", "Lorg/cryo/core/entity/World;");
  onAttachedToWorld.call(csJava::Get(), world->GetJObject());
}


void csJavaState::OnDetachedFromWorld(cs::csWorld *world)
{
  static csJavaCallVoid1<jobject> onDetachedFromWorld (csJava::Get(), this, THIS_CLASS_NAME, "onDetachedFromWorld", "Lorg/cryo/core/entity/World;");
  onDetachedFromWorld.call(csJava::Get(), world->GetJObject());
}


void csJavaState::Update(float tpf)
{
  static csJavaCallVoid1<jfloat> update (csJava::Get(), this, THIS_CLASS_NAME, "update", JAVA_FLOAT);
  update.call(csJava::Get(), tpf);
}

}
