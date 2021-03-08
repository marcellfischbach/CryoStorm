spc::opengl::GL4PointLightClass *spc::opengl::GL4PointLightClass::Get()
{
  static spc::opengl::GL4PointLightClass static_class;
  return &static_class;
};

spc::opengl::GL4PointLightClass::GL4PointLightClass()
  : spc::Class("spc::opengl::GL4PointLight")
{
  AddSuperClass(iPointLight::GetStaticClass());
}

spc::iObject *spc::opengl::GL4PointLightClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4PointLight());
}

const spc::Class *spc::opengl::GL4PointLight::GetClass() const
{
  return spc::opengl::GL4PointLightClass::Get();
}

const spc::Class *spc::opengl::GL4PointLight::GetStaticClass()
{
  return spc::opengl::GL4PointLightClass::Get();
}

void *spc::opengl::GL4PointLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4PointLightClass::Get())
  {
    return static_cast<spc::opengl::GL4PointLight*>(this);
  }
  void *super = nullptr;
  super = iPointLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4PointLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4PointLightClass::Get())
  {
    return static_cast<const spc::opengl::GL4PointLight*>(this);
  }
  const void *super = nullptr;
  super = iPointLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

