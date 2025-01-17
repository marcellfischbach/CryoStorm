#include <csCore/csObjectRegistry.hh>


namespace cs
{

std::map<const csClass*, csRef<iObject>> csObjectRegistry::m_registry;



void csObjectRegistry::Register(const csClass *cls, iObject *obj)
{

  auto it = m_registry.find(cls);
  if (it != m_registry.end())
  {
    m_registry.erase(it);
  }
  m_registry[cls] = obj;
}

iObject *csObjectRegistry::Get(const csClass *cls)
{
  auto it = m_registry.find(cls);
  if (it == m_registry.end())
  {
    return nullptr;
  }
  return it->second;
}


}


#ifdef CS_JAVA

#include <csCore/csClassRegistry.hh>

extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_cryo_core_ObjectRegistry_nGet(JNIEnv *env, jclass, jstring clsName)
{
  const char *clsString = env->GetStringUTFChars(clsName, 0);
  const cs::csClass *cls = cs::csClassRegistry::Get()->GetClass(clsString);
  env->ReleaseStringUTFChars(clsName, clsString);

  cs::iObject *obj = cs::csObjectRegistry::Get(cls);
  return obj ? obj->GetJObject() : nullptr;
}

}
#endif