spc::EntityStateClass *spc::EntityStateClass::Get()
{
  static spc::EntityStateClass static_class;
  return &static_class;
};

spc::EntityStateClass::EntityStateClass()
  : spc::Class("spc::EntityState")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::EntityStateClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::EntityState());
}

const spc::Class *spc::EntityState::GetClass() const
{
  return spc::EntityStateClass::Get();
}

const spc::Class *spc::EntityState::GetStaticClass()
{
  return spc::EntityStateClass::Get();
}

void *spc::EntityState::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::EntityStateClass::Get())
  {
    return static_cast<spc::EntityState*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::EntityState::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::EntityStateClass::Get())
  {
    return static_cast<const spc::EntityState*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

