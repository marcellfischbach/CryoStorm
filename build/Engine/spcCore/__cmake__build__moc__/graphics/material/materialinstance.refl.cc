spc::MaterialInstanceClass *spc::MaterialInstanceClass::Get()
{
  static spc::MaterialInstanceClass static_class;
  return &static_class;
};

spc::MaterialInstanceClass::MaterialInstanceClass()
  : spc::Class("spc::MaterialInstance")
{
  AddSuperClass(iMaterial::GetStaticClass());
}

spc::iObject *spc::MaterialInstanceClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::MaterialInstance());
}

const spc::Class *spc::MaterialInstance::GetClass() const
{
  return spc::MaterialInstanceClass::Get();
}

const spc::Class *spc::MaterialInstance::GetStaticClass()
{
  return spc::MaterialInstanceClass::Get();
}

void *spc::MaterialInstance::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::MaterialInstanceClass::Get())
  {
    return static_cast<spc::MaterialInstance*>(this);
  }
  void *super = nullptr;
  super = iMaterial::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::MaterialInstance::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::MaterialInstanceClass::Get())
  {
    return static_cast<const spc::MaterialInstance*>(this);
  }
  const void *super = nullptr;
  super = iMaterial::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

