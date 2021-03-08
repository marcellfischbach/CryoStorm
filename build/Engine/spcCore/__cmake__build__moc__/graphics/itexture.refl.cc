spc::iTextureClass *spc::iTextureClass::Get()
{
  static spc::iTextureClass static_class;
  return &static_class;
};

spc::iTextureClass::iTextureClass()
  : spc::Class("spc::iTexture")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iTextureClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iTexture");
}

const spc::Class *spc::iTexture::GetClass() const
{
  return spc::iTextureClass::Get();
}

const spc::Class *spc::iTexture::GetStaticClass()
{
  return spc::iTextureClass::Get();
}

void *spc::iTexture::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iTextureClass::Get())
  {
    return static_cast<spc::iTexture*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iTexture::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iTextureClass::Get())
  {
    return static_cast<const spc::iTexture*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

