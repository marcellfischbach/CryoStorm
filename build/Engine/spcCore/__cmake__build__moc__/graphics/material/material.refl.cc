spc::MaterialClass *spc::MaterialClass::Get()
{
  static spc::MaterialClass static_class;
  return &static_class;
};

spc::MaterialClass::MaterialClass()
  : spc::Class("spc::Material")
{
  AddSuperClass(iMaterial::GetStaticClass());
}

spc::iObject *spc::MaterialClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Material());
}

const spc::Class *spc::Material::GetClass() const
{
  return spc::MaterialClass::Get();
}

const spc::Class *spc::Material::GetStaticClass()
{
  return spc::MaterialClass::Get();
}

void *spc::Material::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::MaterialClass::Get())
  {
    return static_cast<spc::Material*>(this);
  }
  void *super = nullptr;
  super = iMaterial::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Material::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::MaterialClass::Get())
  {
    return static_cast<const spc::Material*>(this);
  }
  const void *super = nullptr;
  super = iMaterial::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

