spc::GfxLightClass *spc::GfxLightClass::Get()
{
  static spc::GfxLightClass static_class;
  return &static_class;
};

spc::GfxLightClass::GfxLightClass()
  : spc::Class("spc::GfxLight")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::GfxLightClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::GfxLight());
}

const spc::Class *spc::GfxLight::GetClass() const
{
  return spc::GfxLightClass::Get();
}

const spc::Class *spc::GfxLight::GetStaticClass()
{
  return spc::GfxLightClass::Get();
}

void *spc::GfxLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::GfxLightClass::Get())
  {
    return static_cast<spc::GfxLight*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::GfxLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::GfxLightClass::Get())
  {
    return static_cast<const spc::GfxLight*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

