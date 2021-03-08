spc::CameraClass *spc::CameraClass::Get()
{
  static spc::CameraClass static_class;
  return &static_class;
};

spc::CameraClass::CameraClass()
  : spc::Class("spc::Camera")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::CameraClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Camera());
}

const spc::Class *spc::Camera::GetClass() const
{
  return spc::CameraClass::Get();
}

const spc::Class *spc::Camera::GetStaticClass()
{
  return spc::CameraClass::Get();
}

void *spc::Camera::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::CameraClass::Get())
  {
    return static_cast<spc::Camera*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Camera::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::CameraClass::Get())
  {
    return static_cast<const spc::Camera*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

