spc::opengl::GL4SamplerClass *spc::opengl::GL4SamplerClass::Get()
{
  static spc::opengl::GL4SamplerClass static_class;
  return &static_class;
};

spc::opengl::GL4SamplerClass::GL4SamplerClass()
  : spc::Class("spc::opengl::GL4Sampler")
{
  AddSuperClass(iSampler::GetStaticClass());
}

spc::iObject *spc::opengl::GL4SamplerClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4Sampler());
}

const spc::Class *spc::opengl::GL4Sampler::GetClass() const
{
  return spc::opengl::GL4SamplerClass::Get();
}

const spc::Class *spc::opengl::GL4Sampler::GetStaticClass()
{
  return spc::opengl::GL4SamplerClass::Get();
}

void *spc::opengl::GL4Sampler::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4SamplerClass::Get())
  {
    return static_cast<spc::opengl::GL4Sampler*>(this);
  }
  void *super = nullptr;
  super = iSampler::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Sampler::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4SamplerClass::Get())
  {
    return static_cast<const spc::opengl::GL4Sampler*>(this);
  }
  const void *super = nullptr;
  super = iSampler::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

