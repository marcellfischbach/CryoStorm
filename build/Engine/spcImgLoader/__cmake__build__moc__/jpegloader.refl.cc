spc::img::JpegLoaderClass *spc::img::JpegLoaderClass::Get()
{
  static spc::img::JpegLoaderClass static_class;
  return &static_class;
};

spc::img::JpegLoaderClass::JpegLoaderClass()
  : spc::Class("spc::img::JpegLoader")
{
  AddSuperClass(iAssetLoader::GetStaticClass());
}

spc::iObject *spc::img::JpegLoaderClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::img::JpegLoader());
}

const spc::Class *spc::img::JpegLoader::GetClass() const
{
  return spc::img::JpegLoaderClass::Get();
}

const spc::Class *spc::img::JpegLoader::GetStaticClass()
{
  return spc::img::JpegLoaderClass::Get();
}

void *spc::img::JpegLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::img::JpegLoaderClass::Get())
  {
    return static_cast<spc::img::JpegLoader*>(this);
  }
  void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::img::JpegLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::img::JpegLoaderClass::Get())
  {
    return static_cast<const spc::img::JpegLoader*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

