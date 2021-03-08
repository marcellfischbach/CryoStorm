spc::iTexture2DArrayClass *spc::iTexture2DArrayClass::Get()
{
  static spc::iTexture2DArrayClass static_class;
  return &static_class;
};

spc::iTexture2DArrayClass::iTexture2DArrayClass()
  : spc::Class("spc::iTexture2DArray")
{
  AddSuperClass(iTexture::GetStaticClass());
}

spc::iObject *spc::iTexture2DArrayClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iTexture2DArray");
}

const spc::Class *spc::iTexture2DArray::GetClass() const
{
  return spc::iTexture2DArrayClass::Get();
}

const spc::Class *spc::iTexture2DArray::GetStaticClass()
{
  return spc::iTexture2DArrayClass::Get();
}

void *spc::iTexture2DArray::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iTexture2DArrayClass::Get())
  {
    return static_cast<spc::iTexture2DArray*>(this);
  }
  void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iTexture2DArray::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iTexture2DArrayClass::Get())
  {
    return static_cast<const spc::iTexture2DArray*>(this);
  }
  const void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

