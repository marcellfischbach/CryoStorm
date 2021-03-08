spc::iShaderClass *spc::iShaderClass::Get()
{
  static spc::iShaderClass static_class;
  return &static_class;
};

spc::iShaderClass::iShaderClass()
  : spc::Class("spc::iShader")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iShaderClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iShader");
}

const spc::Class *spc::iShader::GetClass() const
{
  return spc::iShaderClass::Get();
}

const spc::Class *spc::iShader::GetStaticClass()
{
  return spc::iShaderClass::Get();
}

void *spc::iShader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iShaderClass::Get())
  {
    return static_cast<spc::iShader*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iShader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iShaderClass::Get())
  {
    return static_cast<const spc::iShader*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

