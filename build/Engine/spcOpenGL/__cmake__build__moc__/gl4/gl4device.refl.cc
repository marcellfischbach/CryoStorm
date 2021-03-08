spc::opengl::GL4DeviceClass *spc::opengl::GL4DeviceClass::Get()
{
  static spc::opengl::GL4DeviceClass static_class;
  return &static_class;
};

spc::opengl::GL4DeviceClass::GL4DeviceClass()
  : spc::Class("spc::opengl::GL4Device")
{
  AddSuperClass(iDevice::GetStaticClass());
}

spc::iObject *spc::opengl::GL4DeviceClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4Device());
}

const spc::Class *spc::opengl::GL4Device::GetClass() const
{
  return spc::opengl::GL4DeviceClass::Get();
}

const spc::Class *spc::opengl::GL4Device::GetStaticClass()
{
  return spc::opengl::GL4DeviceClass::Get();
}

void *spc::opengl::GL4Device::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4DeviceClass::Get())
  {
    return static_cast<spc::opengl::GL4Device*>(this);
  }
  void *super = nullptr;
  super = iDevice::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4Device::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4DeviceClass::Get())
  {
    return static_cast<const spc::opengl::GL4Device*>(this);
  }
  const void *super = nullptr;
  super = iDevice::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

