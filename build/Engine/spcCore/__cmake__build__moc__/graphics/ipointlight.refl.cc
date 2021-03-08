spc::iPointLightClass *spc::iPointLightClass::Get()
{
  static spc::iPointLightClass static_class;
  return &static_class;
};

spc::iPointLightClass::iPointLightClass()
  : spc::Class("spc::iPointLight")
{
  AddSuperClass(iLight::GetStaticClass());
}

spc::iObject *spc::iPointLightClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iPointLight");
}

const spc::Class *spc::iPointLight::GetClass() const
{
  return spc::iPointLightClass::Get();
}

const spc::Class *spc::iPointLight::GetStaticClass()
{
  return spc::iPointLightClass::Get();
}

void *spc::iPointLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iPointLightClass::Get())
  {
    return static_cast<spc::iPointLight*>(this);
  }
  void *super = nullptr;
  super = iLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iPointLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iPointLightClass::Get())
  {
    return static_cast<const spc::iPointLight*>(this);
  }
  const void *super = nullptr;
  super = iLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

