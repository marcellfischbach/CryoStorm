spc::iTextureCubeClass *spc::iTextureCubeClass::Get()
{
  static spc::iTextureCubeClass static_class;
  return &static_class;
};

spc::iTextureCubeClass::iTextureCubeClass()
  : spc::Class("spc::iTextureCube")
{
  AddSuperClass(iTexture::GetStaticClass());
}

spc::iObject *spc::iTextureCubeClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iTextureCube");
}

const spc::Class *spc::iTextureCube::GetClass() const
{
  return spc::iTextureCubeClass::Get();
}

const spc::Class *spc::iTextureCube::GetStaticClass()
{
  return spc::iTextureCubeClass::Get();
}

void *spc::iTextureCube::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iTextureCubeClass::Get())
  {
    return static_cast<spc::iTextureCube*>(this);
  }
  void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iTextureCube::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iTextureCubeClass::Get())
  {
    return static_cast<const spc::iTextureCube*>(this);
  }
  const void *super = nullptr;
  super = iTexture::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

