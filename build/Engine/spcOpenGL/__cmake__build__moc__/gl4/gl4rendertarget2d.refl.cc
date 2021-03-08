spc::opengl::GL4RenderTarget2DClass *spc::opengl::GL4RenderTarget2DClass::Get()
{
  static spc::opengl::GL4RenderTarget2DClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderTarget2DClass::GL4RenderTarget2DClass()
  : spc::Class("spc::opengl::GL4RenderTarget2D")
{
  AddSuperClass(iRenderTarget2D::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderTarget2DClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4RenderTarget2D());
}

const spc::Class *spc::opengl::GL4RenderTarget2D::GetClass() const
{
  return spc::opengl::GL4RenderTarget2DClass::Get();
}

const spc::Class *spc::opengl::GL4RenderTarget2D::GetStaticClass()
{
  return spc::opengl::GL4RenderTarget2DClass::Get();
}

void *spc::opengl::GL4RenderTarget2D::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderTarget2DClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderTarget2D*>(this);
  }
  void *super = nullptr;
  super = iRenderTarget2D::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderTarget2D::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderTarget2DClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderTarget2D*>(this);
  }
  const void *super = nullptr;
  super = iRenderTarget2D::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

