spc::opengl::GL4ProgramLoaderClass *spc::opengl::GL4ProgramLoaderClass::Get()
{
  static spc::opengl::GL4ProgramLoaderClass static_class;
  return &static_class;
};

spc::opengl::GL4ProgramLoaderClass::GL4ProgramLoaderClass()
  : spc::Class("spc::opengl::GL4ProgramLoader")
{
  AddSuperClass(iAssetLoaderSpc::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ProgramLoaderClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4ProgramLoader());
}

const spc::Class *spc::opengl::GL4ProgramLoader::GetClass() const
{
  return spc::opengl::GL4ProgramLoaderClass::Get();
}

const spc::Class *spc::opengl::GL4ProgramLoader::GetStaticClass()
{
  return spc::opengl::GL4ProgramLoaderClass::Get();
}

void *spc::opengl::GL4ProgramLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ProgramLoaderClass::Get())
  {
    return static_cast<spc::opengl::GL4ProgramLoader*>(this);
  }
  void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4ProgramLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ProgramLoaderClass::Get())
  {
    return static_cast<const spc::opengl::GL4ProgramLoader*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

