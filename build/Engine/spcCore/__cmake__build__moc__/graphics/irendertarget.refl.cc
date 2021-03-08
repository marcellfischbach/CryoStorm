spc::iRenderTargetClass *spc::iRenderTargetClass::Get()
{
  static spc::iRenderTargetClass static_class;
  return &static_class;
};

spc::iRenderTargetClass::iRenderTargetClass()
  : spc::Class("spc::iRenderTarget")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iRenderTargetClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderTarget");
}

const spc::Class *spc::iRenderTarget::GetClass() const
{
  return spc::iRenderTargetClass::Get();
}

const spc::Class *spc::iRenderTarget::GetStaticClass()
{
  return spc::iRenderTargetClass::Get();
}

void *spc::iRenderTarget::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderTargetClass::Get())
  {
    return static_cast<spc::iRenderTarget*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderTarget::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderTargetClass::Get())
  {
    return static_cast<const spc::iRenderTarget*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

