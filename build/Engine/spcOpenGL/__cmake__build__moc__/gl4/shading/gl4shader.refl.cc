spc::opengl::GL4ShaderClass *spc::opengl::GL4ShaderClass::Get()
{
  static spc::opengl::GL4ShaderClass static_class;
  return &static_class;
};

spc::opengl::GL4ShaderClass::GL4ShaderClass()
  : spc::Class("spc::opengl::GL4Shader")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ShaderClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::opengl::GL4Shader");
}

const spc::Class *spc::opengl::GL4Shader::GetClass() const
{
  return spc::opengl::GL4ShaderClass::Get();
}

const spc::Class *spc::opengl::GL4Shader::GetStaticClass()
{
  return spc::opengl::GL4ShaderClass::Get();
}

void *spc::opengl::GL4Shader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ShaderClass::Get())
  {
    return static_cast<spc::opengl::GL4Shader*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Shader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ShaderClass::Get())
  {
    return static_cast<const spc::opengl::GL4Shader*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

