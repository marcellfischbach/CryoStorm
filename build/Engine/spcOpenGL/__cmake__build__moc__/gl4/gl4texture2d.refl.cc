spc::opengl::GL4Texture2DClass *spc::opengl::GL4Texture2DClass::Get()
{
  static spc::opengl::GL4Texture2DClass static_class;
  return &static_class;
};

spc::opengl::GL4Texture2DClass::GL4Texture2DClass()
  : spc::Class("spc::opengl::GL4Texture2D")
{
  AddSuperClass(iTexture2D::GetStaticClass());
}

spc::iObject *spc::opengl::GL4Texture2DClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4Texture2D());
}

const spc::Class *spc::opengl::GL4Texture2D::GetClass() const
{
  return spc::opengl::GL4Texture2DClass::Get();
}

const spc::Class *spc::opengl::GL4Texture2D::GetStaticClass()
{
  return spc::opengl::GL4Texture2DClass::Get();
}

void *spc::opengl::GL4Texture2D::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4Texture2DClass::Get())
  {
    return static_cast<spc::opengl::GL4Texture2D*>(this);
  }
  void *super = nullptr;
  super = iTexture2D::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Texture2D::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4Texture2DClass::Get())
  {
    return static_cast<const spc::opengl::GL4Texture2D*>(this);
  }
  const void *super = nullptr;
  super = iTexture2D::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

