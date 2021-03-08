spc::SamplerLoaderSpcClass *spc::SamplerLoaderSpcClass::Get()
{
  static spc::SamplerLoaderSpcClass static_class;
  return &static_class;
};

spc::SamplerLoaderSpcClass::SamplerLoaderSpcClass()
  : spc::Class("spc::SamplerLoaderSpc")
{
  AddSuperClass(iAssetLoaderSpc::GetStaticClass());
}

spc::iObject *spc::SamplerLoaderSpcClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::SamplerLoaderSpc());
}

const spc::Class *spc::SamplerLoaderSpc::GetClass() const
{
  return spc::SamplerLoaderSpcClass::Get();
}

const spc::Class *spc::SamplerLoaderSpc::GetStaticClass()
{
  return spc::SamplerLoaderSpcClass::Get();
}

void *spc::SamplerLoaderSpc::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::SamplerLoaderSpcClass::Get())
  {
    return static_cast<spc::SamplerLoaderSpc*>(this);
  }
  void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::SamplerLoaderSpc::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::SamplerLoaderSpcClass::Get())
  {
    return static_cast<const spc::SamplerLoaderSpc*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoaderSpc::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

