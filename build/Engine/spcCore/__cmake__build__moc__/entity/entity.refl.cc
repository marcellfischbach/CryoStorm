spc::EntityClass *spc::EntityClass::Get()
{
  static spc::EntityClass static_class;
  return &static_class;
};

spc::EntityClass::EntityClass()
  : spc::Class("spc::Entity")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::EntityClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Entity());
}

const spc::Class *spc::Entity::GetClass() const
{
  return spc::EntityClass::Get();
}

const spc::Class *spc::Entity::GetStaticClass()
{
  return spc::EntityClass::Get();
}

void *spc::Entity::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::EntityClass::Get())
  {
    return static_cast<spc::Entity*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Entity::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::EntityClass::Get())
  {
    return static_cast<const spc::Entity*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

