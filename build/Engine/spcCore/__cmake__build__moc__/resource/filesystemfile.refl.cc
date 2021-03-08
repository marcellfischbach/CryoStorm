spc::FileSystemFileClass *spc::FileSystemFileClass::Get()
{
  static spc::FileSystemFileClass static_class;
  return &static_class;
};

spc::FileSystemFileClass::FileSystemFileClass()
  : spc::Class("spc::FileSystemFile")
{
  AddSuperClass(iFile::GetStaticClass());
}

spc::iObject *spc::FileSystemFileClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::FileSystemFile");
}

const spc::Class *spc::FileSystemFile::GetClass() const
{
  return spc::FileSystemFileClass::Get();
}

const spc::Class *spc::FileSystemFile::GetStaticClass()
{
  return spc::FileSystemFileClass::Get();
}

void *spc::FileSystemFile::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::FileSystemFileClass::Get())
  {
    return static_cast<spc::FileSystemFile*>(this);
  }
  void *super = nullptr;
  super = iFile::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::FileSystemFile::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::FileSystemFileClass::Get())
  {
    return static_cast<const spc::FileSystemFile*>(this);
  }
  const void *super = nullptr;
  super = iFile::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

