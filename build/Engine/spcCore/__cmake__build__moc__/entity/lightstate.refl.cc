spc::LightStateClass *spc::LightStateClass::Get()
{
  static spc::LightStateClass static_class;
  return &static_class;
};

spc::LightStateClass::LightStateClass()
  : spc::Class("spc::LightState")
{
  AddSuperClass(SpatialState::GetStaticClass());
}

spc::iObject *spc::LightStateClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::LightState");
}

const spc::Class *spc::LightState::GetClass() const
{
  return spc::LightStateClass::Get();
}

const spc::Class *spc::LightState::GetStaticClass()
{
  return spc::LightStateClass::Get();
}

void *spc::LightState::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::LightStateClass::Get())
  {
    return static_cast<spc::LightState*>(this);
  }
  void *super = nullptr;
  super = SpatialState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::LightState::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::LightStateClass::Get())
  {
    return static_cast<const spc::LightState*>(this);
  }
  const void *super = nullptr;
  super = SpatialState::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

