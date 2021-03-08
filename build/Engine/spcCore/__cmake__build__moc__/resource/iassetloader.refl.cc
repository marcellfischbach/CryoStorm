spc::iAssetLoaderClass *spc::iAssetLoaderClass::Get()
{
  static spc::iAssetLoaderClass static_class;
  return &static_class;
};

spc::iAssetLoaderClass::iAssetLoaderClass()
  : spc::Class("spc::iAssetLoader")
{
}

spc::iObject *spc::iAssetLoaderClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iAssetLoader");
}

const spc::Class *spc::iAssetLoader::GetClass() const
{
  return spc::iAssetLoaderClass::Get();
}

const spc::Class *spc::iAssetLoader::GetStaticClass()
{
  return spc::iAssetLoaderClass::Get();
}

void *spc::iAssetLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iAssetLoaderClass::Get())
  {
    return static_cast<spc::iAssetLoader*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iAssetLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iAssetLoaderClass::Get())
  {
    return static_cast<const spc::iAssetLoader*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::iAssetLoaderSpcClass *spc::iAssetLoaderSpcClass::Get()
{
  static spc::iAssetLoaderSpcClass static_class;
  return &static_class;
};

spc::iAssetLoaderSpcClass::iAssetLoaderSpcClass()
  : spc::Class("spc::iAssetLoaderSpc")
{
}

spc::iObject *spc::iAssetLoaderSpcClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iAssetLoaderSpc");
}

const spc::Class *spc::iAssetLoaderSpc::GetClass() const
{
  return spc::iAssetLoaderSpcClass::Get();
}

const spc::Class *spc::iAssetLoaderSpc::GetStaticClass()
{
  return spc::iAssetLoaderSpcClass::Get();
}

void *spc::iAssetLoaderSpc::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iAssetLoaderSpcClass::Get())
  {
    return static_cast<spc::iAssetLoaderSpc*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iAssetLoaderSpc::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iAssetLoaderSpcClass::Get())
  {
    return static_cast<const spc::iAssetLoaderSpc*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

