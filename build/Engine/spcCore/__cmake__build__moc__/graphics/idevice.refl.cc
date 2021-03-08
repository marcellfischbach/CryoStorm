spc::iDeviceClass *spc::iDeviceClass::Get()
{
  static spc::iDeviceClass static_class;
  return &static_class;
};

spc::iDeviceClass::iDeviceClass()
  : spc::Class("spc::iDevice")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iDeviceClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iDevice");
}

const spc::Class *spc::iDevice::GetClass() const
{
  return spc::iDeviceClass::Get();
}

const spc::Class *spc::iDevice::GetStaticClass()
{
  return spc::iDeviceClass::Get();
}

void *spc::iDevice::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iDeviceClass::Get())
  {
    return static_cast<spc::iDevice*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iDevice::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iDeviceClass::Get())
  {
    return static_cast<const spc::iDevice*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

