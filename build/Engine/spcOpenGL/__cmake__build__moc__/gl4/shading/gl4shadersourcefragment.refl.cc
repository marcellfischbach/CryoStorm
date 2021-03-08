spc::opengl::GL4ShaderSourceFragmentClass *spc::opengl::GL4ShaderSourceFragmentClass::Get()
{
  static spc::opengl::GL4ShaderSourceFragmentClass static_class;
  return &static_class;
};

spc::opengl::GL4ShaderSourceFragmentClass::GL4ShaderSourceFragmentClass()
  : spc::Class("spc::opengl::GL4ShaderSourceFragment")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ShaderSourceFragmentClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4ShaderSourceFragment());
}

const spc::Class *spc::opengl::GL4ShaderSourceFragment::GetClass() const
{
  return spc::opengl::GL4ShaderSourceFragmentClass::Get();
}

const spc::Class *spc::opengl::GL4ShaderSourceFragment::GetStaticClass()
{
  return spc::opengl::GL4ShaderSourceFragmentClass::Get();
}

void *spc::opengl::GL4ShaderSourceFragment::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ShaderSourceFragmentClass::Get())
  {
    return static_cast<spc::opengl::GL4ShaderSourceFragment*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4ShaderSourceFragment::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ShaderSourceFragmentClass::Get())
  {
    return static_cast<const spc::opengl::GL4ShaderSourceFragment*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

