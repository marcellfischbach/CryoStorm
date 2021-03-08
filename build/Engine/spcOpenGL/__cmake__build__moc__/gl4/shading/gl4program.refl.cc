spc::opengl::GL4ProgramClass *spc::opengl::GL4ProgramClass::Get()
{
  static spc::opengl::GL4ProgramClass static_class;
  return &static_class;
};

spc::opengl::GL4ProgramClass::GL4ProgramClass()
  : spc::Class("spc::opengl::GL4Program")
{
  AddSuperClass(iShader::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ProgramClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4Program());
}

const spc::Class *spc::opengl::GL4Program::GetClass() const
{
  return spc::opengl::GL4ProgramClass::Get();
}

const spc::Class *spc::opengl::GL4Program::GetStaticClass()
{
  return spc::opengl::GL4ProgramClass::Get();
}

void *spc::opengl::GL4Program::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ProgramClass::Get())
  {
    return static_cast<spc::opengl::GL4Program*>(this);
  }
  void *super = nullptr;
  super = iShader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Program::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ProgramClass::Get())
  {
    return static_cast<const spc::opengl::GL4Program*>(this);
  }
  const void *super = nullptr;
  super = iShader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

