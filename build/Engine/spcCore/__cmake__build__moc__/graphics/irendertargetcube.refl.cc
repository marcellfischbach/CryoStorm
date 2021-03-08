spc::iRenderTargetCubeClass *spc::iRenderTargetCubeClass::Get()
{
  static spc::iRenderTargetCubeClass static_class;
  return &static_class;
};

spc::iRenderTargetCubeClass::iRenderTargetCubeClass()
  : spc::Class("spc::iRenderTargetCube")
{
  AddSuperClass(iRenderTarget::GetStaticClass());
}

spc::iObject *spc::iRenderTargetCubeClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderTargetCube");
}

const spc::Class *spc::iRenderTargetCube::GetClass() const
{
  return spc::iRenderTargetCubeClass::Get();
}

const spc::Class *spc::iRenderTargetCube::GetStaticClass()
{
  return spc::iRenderTargetCubeClass::Get();
}

void *spc::iRenderTargetCube::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderTargetCubeClass::Get())
  {
    return static_cast<spc::iRenderTargetCube*>(this);
  }
  void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderTargetCube::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderTargetCubeClass::Get())
  {
    return static_cast<const spc::iRenderTargetCube*>(this);
  }
  const void *super = nullptr;
  super = iRenderTarget::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

