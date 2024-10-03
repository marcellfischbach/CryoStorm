
#include <ceCore/entity/javastate.hh>
#include <ceCore/java.hh>
#include <ceCore/java_methods.hh>
#include <ceCore/entity/world.hh>

#define THIS_CLASS_NAME  "org/crimsonedge/core/entity/JavaState"

namespace cryo
{


JavaState::JavaState()
: EntityState()
{
  SetNeedUpdate(true);
}

void JavaState::OnAttachedToWorld(cryo::World *world)
{
  static JavaCallVoid1<jobject> onAttachedToWorld (Java::Get(), this, THIS_CLASS_NAME, "onAttachedToWorld", "Lorg/crimsonedge/core/entity/World;");
  onAttachedToWorld.call(Java::Get(), world->GetJObject());
}


void JavaState::OnDetachedFromWorld(cryo::World *world)
{
  static JavaCallVoid1<jobject> onDetachedFromWorld (Java::Get(), this, THIS_CLASS_NAME, "onDetachedFromWorld", "Lorg/crimsonedge/core/entity/World;");
  onDetachedFromWorld.call(Java::Get(), world->GetJObject());
}


void JavaState::Update(float tpf)
{
  static JavaCallVoid1<jfloat> update (Java::Get(), this, THIS_CLASS_NAME, "update", JAVA_FLOAT);
  update.call(Java::Get(), tpf);
}

}
