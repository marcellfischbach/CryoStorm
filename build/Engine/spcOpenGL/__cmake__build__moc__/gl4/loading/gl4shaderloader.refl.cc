spc::opengl::GL4ShaderLoaderSpcClass *spc::opengl::GL4ShaderLoaderSpcClass::Get()
{
  static spc::opengl::GL4ShaderLoaderSpcClass static_class;
  return &static_class;
};

spc::opengl::GL4ShaderLoaderSpcClass::GL4ShaderLoaderSpcClass()
  : spc::Class("spc::opengl::GL4ShaderLoaderSpc")
{
  AddSuperClass(iAssetLoaderSpc::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ShaderLoaderSpcClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4ShaderLoaderSpc());
}

const spc::Class *spc::opengl::GL4ShaderLoaderSpc::GetClass() const
{
  return spc::opengl::GL4ShaderLoaderSpcClass::Get();
}

const spc::Class *spc::opengl::GL4ShaderLoaderSpc::GetStaticClass()
{
  return spc::opengl::GL4ShaderLoaderSpcClass::Get();
}

void *spc::opengl::GL4ShaderLoaderSpc::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ShaderLoaderSpcClass::Get())
  {
    return static_cast<spc::opengl::GL4ShaderLoaderSpc*>(this);
  }
  void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4ShaderLoaderSpc::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ShaderLoaderSpcClass::Get())
  {
    return static_cast<const spc::opengl::GL4ShaderLoaderSpc*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::opengl::GL4ShaderLoaderClass *spc::opengl::GL4ShaderLoaderClass::Get()
{
  static spc::opengl::GL4ShaderLoaderClass static_class;
  return &static_class;
};

spc::opengl::GL4ShaderLoaderClass::GL4ShaderLoaderClass()
  : spc::Class("spc::opengl::GL4ShaderLoader")
{
  AddSuperClass(iAssetLoader::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ShaderLoaderClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4ShaderLoader());
}

const spc::Class *spc::opengl::GL4ShaderLoader::GetClass() const
{
  return spc::opengl::GL4ShaderLoaderClass::Get();
}

const spc::Class *spc::opengl::GL4ShaderLoader::GetStaticClass()
{
  return spc::opengl::GL4ShaderLoaderClass::Get();
}

void *spc::opengl::GL4ShaderLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ShaderLoaderClass::Get())
  {
    return static_cast<spc::opengl::GL4ShaderLoader*>(this);
  }
  void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4ShaderLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ShaderLoaderClass::Get())
  {
    return static_cast<const spc::opengl::GL4ShaderLoader*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

