spc::iMaterialClass *spc::iMaterialClass::Get()
{
  static spc::iMaterialClass static_class;
  return &static_class;
};

spc::iMaterialClass::iMaterialClass()
  : spc::Class("spc::iMaterial")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iMaterialClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iMaterial");
}

const spc::Class *spc::iMaterial::GetClass() const
{
  return spc::iMaterialClass::Get();
}

const spc::Class *spc::iMaterial::GetStaticClass()
{
  return spc::iMaterialClass::Get();
}

void *spc::iMaterial::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iMaterialClass::Get())
  {
    return static_cast<spc::iMaterial*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iMaterial::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iMaterialClass::Get())
  {
    return static_cast<const spc::iMaterial*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

