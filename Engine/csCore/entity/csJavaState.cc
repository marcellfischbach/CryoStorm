
#include <csCore/entity/csJavaState.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>
#include <csCore/entity/csWorld.hh>

#define THIS_CLASS_NAME  "org/cryo/core/entity/CsJavaState"

namespace cs
{

struct csJavaStatePrivate
{
  csJavaStatePrivate(csJavaState *state)
  {
    JNIEnv *env = csJava::Get();
    onAttachedToWorld = csJavaCallVoid1<jobject>(env,
                                                 state,
                                                 THIS_CLASS_NAME,
                                                 "onAttachedToWorld",
                                                 "Lorg/cryo/core/entity/CsWorld;");
    onDetachedFromWorld = csJavaCallVoid1<jobject>(env,
                                                   state,
                                                   THIS_CLASS_NAME,
                                                   "onDetachedFromWorld",
                                                   "Lorg/cryo/core/entity/CsWorld;");
    update              = csJavaCallVoid1<jfloat>(env, state, THIS_CLASS_NAME, "update", JAVA_FLOAT);

  }

  csJavaCallVoid1<jobject> onAttachedToWorld;
  csJavaCallVoid1<jobject> onDetachedFromWorld;
  csJavaCallVoid1<jfloat>  update;
};

csJavaState::csJavaState()
    : csEntityState()
    , m_priv(new csJavaStatePrivate(this))
{
  SetNeedUpdate(true);
}

csJavaState::~csJavaState()
{
  delete m_priv;
  m_priv = nullptr;
}

void csJavaState::OnAttachedToWorld(cs::csWorld *world)
{
  m_priv->onAttachedToWorld.call(csJava::Get(), world->GetJObject());
}


void csJavaState::OnDetachedFromWorld(cs::csWorld *world)
{
  m_priv->onDetachedFromWorld.call(csJava::Get(), world->GetJObject());
}


void csJavaState::Update(float tpf)
{
  m_priv->update.call(csJava::Get(), tpf);
}

}
