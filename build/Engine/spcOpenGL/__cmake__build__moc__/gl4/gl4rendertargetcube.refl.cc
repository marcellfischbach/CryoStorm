spc::opengl::GL4RenderTargetCubeClass *spc::opengl::GL4RenderTargetCubeClass::Get()
{
  static spc::opengl::GL4RenderTargetCubeClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderTargetCubeClass::GL4RenderTargetCubeClass()
  : spc::Class("spc::opengl::GL4RenderTargetCube")
{
  AddSuperClass(iRenderTargetCube::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderTargetCubeClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4RenderTargetCube());
}

const spc::Class *spc::opengl::GL4RenderTargetCube::GetClass() const
{
  return spc::opengl::GL4RenderTargetCubeClass::Get();
}

const spc::Class *spc::opengl::GL4RenderTargetCube::GetStaticClass()
{
  return spc::opengl::GL4RenderTargetCubeClass::Get();
}

void *spc::opengl::GL4RenderTargetCube::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderTargetCubeClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderTargetCube*>(this);
  }
  void *super = nullptr;
  super = iRenderTargetCube::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderTargetCube::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderTargetCubeClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderTargetCube*>(this);
  }
  const void *super = nullptr;
  super = iRenderTargetCube::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

