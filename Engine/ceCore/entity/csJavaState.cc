
#include <ceCore/entity/csJavaState.hh>
#include <ceCore/java.hh>
#include <ceCore/java_methods.hh>
#include <ceCore/entity/csWorld.hh>

#define THIS_CLASS_NAME  "org/crimsonedge/core/entity/JavaState"

namespace cryo
{


csJavaState::csJavaState()
: csEntityState()
{
  SetNeedUpdate(true);
}

void csJavaState::OnAttachedToWorld(cryo::csWorld *world)
{
  static JavaCallVoid1<jobject> onAttachedToWorld (Java::Get(), this, THIS_CLASS_NAME, "onAttachedToWorld", "Lorg/crimsonedge/core/entity/World;");
  onAttachedToWorld.call(Java::Get(), world->GetJObject());
}


void csJavaState::OnDetachedFromWorld(cryo::csWorld *world)
{
  static JavaCallVoid1<jobject> onDetachedFromWorld (Java::Get(), this, THIS_CLASS_NAME, "onDetachedFromWorld", "Lorg/crimsonedge/core/entity/World;");
  onDetachedFromWorld.call(Java::Get(), world->GetJObject());
}


void csJavaState::Update(float tpf)
{
  static JavaCallVoid1<jfloat> update (Java::Get(), this, THIS_CLASS_NAME, "update", JAVA_FLOAT);
  update.call(Java::Get(), tpf);
}

}
