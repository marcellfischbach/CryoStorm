spc::iSamplerClass *spc::iSamplerClass::Get()
{
  static spc::iSamplerClass static_class;
  return &static_class;
};

spc::iSamplerClass::iSamplerClass()
  : spc::Class("spc::iSampler")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iSamplerClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iSampler");
}

const spc::Class *spc::iSampler::GetClass() const
{
  return spc::iSamplerClass::Get();
}

const spc::Class *spc::iSampler::GetStaticClass()
{
  return spc::iSamplerClass::Get();
}

void *spc::iSampler::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iSamplerClass::Get())
  {
    return static_cast<spc::iSampler*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iSampler::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iSamplerClass::Get())
  {
    return static_cast<const spc::iSampler*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

