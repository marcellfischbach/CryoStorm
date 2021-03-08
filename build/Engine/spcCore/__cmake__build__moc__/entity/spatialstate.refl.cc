spc::SpatialStateClass *spc::SpatialStateClass::Get()
{
  static spc::SpatialStateClass static_class;
  return &static_class;
};

spc::SpatialStateClass::SpatialStateClass()
  : spc::Class("spc::SpatialState")
{
  AddSuperClass(EntityState::GetStaticClass());
}

spc::iObject *spc::SpatialStateClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::SpatialState());
}

const spc::Class *spc::SpatialState::GetClass() const
{
  return spc::SpatialStateClass::Get();
}

const spc::Class *spc::SpatialState::GetStaticClass()
{
  return spc::SpatialStateClass::Get();
}

void *spc::SpatialState::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::SpatialStateClass::Get())
  {
    return static_cast<spc::SpatialState*>(this);
  }
  void *super = nullptr;
  super = EntityState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::SpatialState::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::SpatialStateClass::Get())
  {
    return static_cast<const spc::SpatialState*>(this);
  }
  const void *super = nullptr;
  super = EntityState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

