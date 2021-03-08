spc::iFileClass *spc::iFileClass::Get()
{
  static spc::iFileClass static_class;
  return &static_class;
};

spc::iFileClass::iFileClass()
  : spc::Class("spc::iFile")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iFileClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iFile");
}

const spc::Class *spc::iFile::GetClass() const
{
  return spc::iFileClass::Get();
}

const spc::Class *spc::iFile::GetStaticClass()
{
  return spc::iFileClass::Get();
}

void *spc::iFile::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iFileClass::Get())
  {
    return static_cast<spc::iFile*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iFile::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iFileClass::Get())
  {
    return static_cast<const spc::iFile*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

