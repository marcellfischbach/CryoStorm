spc::opengl::GL4Texture2DArrayClass *spc::opengl::GL4Texture2DArrayClass::Get()
{
  static spc::opengl::GL4Texture2DArrayClass static_class;
  return &static_class;
};

spc::opengl::GL4Texture2DArrayClass::GL4Texture2DArrayClass()
  : spc::Class("spc::opengl::GL4Texture2DArray")
{
  AddSuperClass(iTexture2DArray::GetStaticClass());
}

spc::iObject *spc::opengl::GL4Texture2DArrayClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4Texture2DArray());
}

const spc::Class *spc::opengl::GL4Texture2DArray::GetClass() const
{
  return spc::opengl::GL4Texture2DArrayClass::Get();
}

const spc::Class *spc::opengl::GL4Texture2DArray::GetStaticClass()
{
  return spc::opengl::GL4Texture2DArrayClass::Get();
}

void *spc::opengl::GL4Texture2DArray::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4Texture2DArrayClass::Get())
  {
    return static_cast<spc::opengl::GL4Texture2DArray*>(this);
  }
  void *super = nullptr;
  super = iTexture2DArray::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Texture2DArray::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4Texture2DArrayClass::Get())
  {
    return static_cast<const spc::opengl::GL4Texture2DArray*>(this);
  }
  const void *super = nullptr;
  super = iTexture2DArray::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

