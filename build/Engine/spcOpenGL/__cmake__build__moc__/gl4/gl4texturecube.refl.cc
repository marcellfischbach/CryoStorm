spc::opengl::GL4TextureCubeClass *spc::opengl::GL4TextureCubeClass::Get()
{
  static spc::opengl::GL4TextureCubeClass static_class;
  return &static_class;
};

spc::opengl::GL4TextureCubeClass::GL4TextureCubeClass()
  : spc::Class("spc::opengl::GL4TextureCube")
{
  AddSuperClass(iTextureCube::GetStaticClass());
}

spc::iObject *spc::opengl::GL4TextureCubeClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4TextureCube());
}

const spc::Class *spc::opengl::GL4TextureCube::GetClass() const
{
  return spc::opengl::GL4TextureCubeClass::Get();
}

const spc::Class *spc::opengl::GL4TextureCube::GetStaticClass()
{
  return spc::opengl::GL4TextureCubeClass::Get();
}

void *spc::opengl::GL4TextureCube::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4TextureCubeClass::Get())
  {
    return static_cast<spc::opengl::GL4TextureCube*>(this);
  }
  void *super = nullptr;
  super = iTextureCube::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4TextureCube::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4TextureCubeClass::Get())
  {
    return static_cast<const spc::opengl::GL4TextureCube*>(this);
  }
  const void *super = nullptr;
  super = iTextureCube::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

