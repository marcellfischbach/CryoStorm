spc::iRenderTarget2DClass *spc::iRenderTarget2DClass::Get()
{
  static spc::iRenderTarget2DClass static_class;
  return &static_class;
};

spc::iRenderTarget2DClass::iRenderTarget2DClass()
  : spc::Class("spc::iRenderTarget2D")
{
  AddSuperClass(iRenderTarget::GetStaticClass());
}

spc::iObject *spc::iRenderTarget2DClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderTarget2D");
}

const spc::Class *spc::iRenderTarget2D::GetClass() const
{
  return spc::iRenderTarget2DClass::Get();
}

const spc::Class *spc::iRenderTarget2D::GetStaticClass()
{
  return spc::iRenderTarget2DClass::Get();
}

void *spc::iRenderTarget2D::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderTarget2DClass::Get())
  {
    return static_cast<spc::iRenderTarget2D*>(this);
  }
  void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderTarget2D::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderTarget2DClass::Get())
  {
    return static_cast<const spc::iRenderTarget2D*>(this);
  }
  const void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

