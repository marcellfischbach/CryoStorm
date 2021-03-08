spc::iTexture2DClass *spc::iTexture2DClass::Get()
{
  static spc::iTexture2DClass static_class;
  return &static_class;
};

spc::iTexture2DClass::iTexture2DClass()
  : spc::Class("spc::iTexture2D")
{
  AddSuperClass(iTexture::GetStaticClass());
}

spc::iObject *spc::iTexture2DClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iTexture2D");
}

const spc::Class *spc::iTexture2D::GetClass() const
{
  return spc::iTexture2DClass::Get();
}

const spc::Class *spc::iTexture2D::GetStaticClass()
{
  return spc::iTexture2DClass::Get();
}

void *spc::iTexture2D::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iTexture2DClass::Get())
  {
    return static_cast<spc::iTexture2D*>(this);
  }
  void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iTexture2D::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iTexture2DClass::Get())
  {
    return static_cast<const spc::iTexture2D*>(this);
  }
  const void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

