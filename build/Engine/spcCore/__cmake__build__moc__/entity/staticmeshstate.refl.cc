spc::StaticMeshStateClass *spc::StaticMeshStateClass::Get()
{
  static spc::StaticMeshStateClass static_class;
  return &static_class;
};

spc::StaticMeshStateClass::StaticMeshStateClass()
  : spc::Class("spc::StaticMeshState")
{
  AddSuperClass(SpatialState::GetStaticClass());
}

spc::iObject *spc::StaticMeshStateClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::StaticMeshState());
}

const spc::Class *spc::StaticMeshState::GetClass() const
{
  return spc::StaticMeshStateClass::Get();
}

const spc::Class *spc::StaticMeshState::GetStaticClass()
{
  return spc::StaticMeshStateClass::Get();
}

void *spc::StaticMeshState::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::StaticMeshStateClass::Get())
  {
    return static_cast<spc::StaticMeshState*>(this);
  }
  void *super = nullptr;
  super = SpatialState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::StaticMeshState::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::StaticMeshStateClass::Get())
  {
    return static_cast<const spc::StaticMeshState*>(this);
  }
  const void *super = nullptr;
  super = SpatialState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

