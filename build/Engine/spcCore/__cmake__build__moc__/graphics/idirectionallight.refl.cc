spc::iDirectionalLightClass *spc::iDirectionalLightClass::Get()
{
  static spc::iDirectionalLightClass static_class;
  return &static_class;
};

spc::iDirectionalLightClass::iDirectionalLightClass()
  : spc::Class("spc::iDirectionalLight")
{
  AddSuperClass(iLight::GetStaticClass());
}

spc::iObject *spc::iDirectionalLightClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iDirectionalLight");
}

const spc::Class *spc::iDirectionalLight::GetClass() const
{
  return spc::iDirectionalLightClass::Get();
}

const spc::Class *spc::iDirectionalLight::GetStaticClass()
{
  return spc::iDirectionalLightClass::Get();
}

void *spc::iDirectionalLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iDirectionalLightClass::Get())
  {
    return static_cast<spc::iDirectionalLight*>(this);
  }
  void *super = nullptr;
  super = iLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iDirectionalLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iDirectionalLightClass::Get())
  {
    return static_cast<const spc::iDirectionalLight*>(this);
  }
  const void *super = nullptr;
  super = iLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

