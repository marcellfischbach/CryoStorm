spc::opengl::GL4RenderTarget2DArrayClass *spc::opengl::GL4RenderTarget2DArrayClass::Get()
{
  static spc::opengl::GL4RenderTarget2DArrayClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderTarget2DArrayClass::GL4RenderTarget2DArrayClass()
  : spc::Class("spc::opengl::GL4RenderTarget2DArray")
{
  AddSuperClass(iRenderTarget2DArray::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderTarget2DArrayClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4RenderTarget2DArray());
}

const spc::Class *spc::opengl::GL4RenderTarget2DArray::GetClass() const
{
  return spc::opengl::GL4RenderTarget2DArrayClass::Get();
}

const spc::Class *spc::opengl::GL4RenderTarget2DArray::GetStaticClass()
{
  return spc::opengl::GL4RenderTarget2DArrayClass::Get();
}

void *spc::opengl::GL4RenderTarget2DArray::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderTarget2DArrayClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderTarget2DArray*>(this);
  }
  void *super = nullptr;
  super = iRenderTarget2DArray::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderTarget2DArray::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderTarget2DArrayClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderTarget2DArray*>(this);
  }
  const void *super = nullptr;
  super = iRenderTarget2DArray::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

