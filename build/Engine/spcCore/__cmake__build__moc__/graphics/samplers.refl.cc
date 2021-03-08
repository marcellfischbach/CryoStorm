spc::SamplersClass *spc::SamplersClass::Get()
{
  static spc::SamplersClass static_class;
  return &static_class;
};

spc::SamplersClass::SamplersClass()
  : spc::Class("spc::Samplers")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::SamplersClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Samplers());
}

const spc::Class *spc::Samplers::GetClass() const
{
  return spc::SamplersClass::Get();
}

const spc::Class *spc::Samplers::GetStaticClass()
{
  return spc::SamplersClass::Get();
}

void *spc::Samplers::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::SamplersClass::Get())
  {
    return static_cast<spc::Samplers*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Samplers::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::SamplersClass::Get())
  {
    return static_cast<const spc::Samplers*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

