spc::GfxSceneClass *spc::GfxSceneClass::Get()
{
  static spc::GfxSceneClass static_class;
  return &static_class;
};

spc::GfxSceneClass::GfxSceneClass()
  : spc::Class("spc::GfxScene")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::GfxSceneClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::GfxScene());
}

const spc::Class *spc::GfxScene::GetClass() const
{
  return spc::GfxSceneClass::Get();
}

const spc::Class *spc::GfxScene::GetStaticClass()
{
  return spc::GfxSceneClass::Get();
}

void *spc::GfxScene::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::GfxSceneClass::Get())
  {
    return static_cast<spc::GfxScene*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::GfxScene::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::GfxSceneClass::Get())
  {
    return static_cast<const spc::GfxScene*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

