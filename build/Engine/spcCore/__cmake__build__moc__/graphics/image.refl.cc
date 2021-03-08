spc::ImageClass *spc::ImageClass::Get()
{
  static spc::ImageClass static_class;
  return &static_class;
};

spc::ImageClass::ImageClass()
  : spc::Class("spc::Image")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::ImageClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::Image");
}

const spc::Class *spc::Image::GetClass() const
{
  return spc::ImageClass::Get();
}

const spc::Class *spc::Image::GetStaticClass()
{
  return spc::ImageClass::Get();
}

void *spc::Image::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::ImageClass::Get())
  {
    return static_cast<spc::Image*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Image::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::ImageClass::Get())
  {
    return static_cast<const spc::Image*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

