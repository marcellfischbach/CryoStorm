spc::img::PngLoaderClass *spc::img::PngLoaderClass::Get()
{
  static spc::img::PngLoaderClass static_class;
  return &static_class;
};

spc::img::PngLoaderClass::PngLoaderClass()
  : spc::Class("spc::img::PngLoader")
{
  AddSuperClass(iAssetLoader::GetStaticClass());
}

spc::iObject *spc::img::PngLoaderClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::img::PngLoader());
}

const spc::Class *spc::img::PngLoader::GetClass() const
{
  return spc::img::PngLoaderClass::Get();
}

const spc::Class *spc::img::PngLoader::GetStaticClass()
{
  return spc::img::PngLoaderClass::Get();
}

void *spc::img::PngLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::img::PngLoaderClass::Get())
  {
    return static_cast<spc::img::PngLoader*>(this);
  }
  void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::img::PngLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::img::PngLoaderClass::Get())
  {
    return static_cast<const spc::img::PngLoader*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

