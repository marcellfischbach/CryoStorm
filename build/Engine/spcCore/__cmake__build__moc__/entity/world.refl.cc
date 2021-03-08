spc::WorldClass *spc::WorldClass::Get()
{
  static spc::WorldClass static_class;
  return &static_class;
};

spc::WorldClass::WorldClass()
  : spc::Class("spc::World")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::WorldClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::World());
}

const spc::Class *spc::World::GetClass() const
{
  return spc::WorldClass::Get();
}

const spc::Class *spc::World::GetStaticClass()
{
  return spc::WorldClass::Get();
}

void *spc::World::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::WorldClass::Get())
  {
    return static_cast<spc::World*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::World::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::WorldClass::Get())
  {
    return static_cast<const spc::World*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

