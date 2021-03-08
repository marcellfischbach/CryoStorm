spc::iRenderTarget2DArrayClass *spc::iRenderTarget2DArrayClass::Get()
{
  static spc::iRenderTarget2DArrayClass static_class;
  return &static_class;
};

spc::iRenderTarget2DArrayClass::iRenderTarget2DArrayClass()
  : spc::Class("spc::iRenderTarget2DArray")
{
  AddSuperClass(iRenderTarget::GetStaticClass());
}

spc::iObject *spc::iRenderTarget2DArrayClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderTarget2DArray");
}

const spc::Class *spc::iRenderTarget2DArray::GetClass() const
{
  return spc::iRenderTarget2DArrayClass::Get();
}

const spc::Class *spc::iRenderTarget2DArray::GetStaticClass()
{
  return spc::iRenderTarget2DArrayClass::Get();
}

void *spc::iRenderTarget2DArray::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderTarget2DArrayClass::Get())
  {
    return static_cast<spc::iRenderTarget2DArray*>(this);
  }
  void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderTarget2DArray::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderTarget2DArrayClass::Get())
  {
    return static_cast<const spc::iRenderTarget2DArray*>(this);
  }
  const void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

