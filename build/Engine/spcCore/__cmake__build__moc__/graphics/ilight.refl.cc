spc::iLightClass *spc::iLightClass::Get()
{
  static spc::iLightClass static_class;
  return &static_class;
};

spc::iLightClass::iLightClass()
  : spc::Class("spc::iLight")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iLightClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iLight");
}

const spc::Class *spc::iLight::GetClass() const
{
  return spc::iLightClass::Get();
}

const spc::Class *spc::iLight::GetStaticClass()
{
  return spc::iLightClass::Get();
}

void *spc::iLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iLightClass::Get())
  {
    return static_cast<spc::iLight*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iLightClass::Get())
  {
    return static_cast<const spc::iLight*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

