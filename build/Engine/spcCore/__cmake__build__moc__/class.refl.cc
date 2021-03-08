spc::ObjectClass *spc::ObjectClass::Get()
{
  static spc::ObjectClass static_class;
  return &static_class;
};

spc::ObjectClass::ObjectClass()
  : spc::Class("spc::Object")
{
  AddSuperClass(spc::iObject::GetStaticClass());
}

spc::iObject *spc::ObjectClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Object());
}

const spc::Class *spc::Object::GetClass() const
{
  return spc::ObjectClass::Get();
}

const spc::Class *spc::Object::GetStaticClass()
{
  return spc::ObjectClass::Get();
}

void *spc::Object::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::ObjectClass::Get())
  {
    return static_cast<spc::Object*>(this);
  }
  void *super = nullptr;
  super = spc::iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Object::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::ObjectClass::Get())
  {
    return static_cast<const spc::Object*>(this);
  }
  const void *super = nullptr;
  super = spc::iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

