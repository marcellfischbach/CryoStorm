spc::opengl::GL4DirectionalLightClass *spc::opengl::GL4DirectionalLightClass::Get()
{
  static spc::opengl::GL4DirectionalLightClass static_class;
  return &static_class;
};

spc::opengl::GL4DirectionalLightClass::GL4DirectionalLightClass()
  : spc::Class("spc::opengl::GL4DirectionalLight")
{
  AddSuperClass(iDirectionalLight::GetStaticClass());
}

spc::iObject *spc::opengl::GL4DirectionalLightClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4DirectionalLight());
}

const spc::Class *spc::opengl::GL4DirectionalLight::GetClass() const
{
  return spc::opengl::GL4DirectionalLightClass::Get();
}

const spc::Class *spc::opengl::GL4DirectionalLight::GetStaticClass()
{
  return spc::opengl::GL4DirectionalLightClass::Get();
}

void *spc::opengl::GL4DirectionalLight::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4DirectionalLightClass::Get())
  {
    return static_cast<spc::opengl::GL4DirectionalLight*>(this);
  }
  void *super = nullptr;
  super = iDirectionalLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4DirectionalLight::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4DirectionalLightClass::Get())
  {
    return static_cast<const spc::opengl::GL4DirectionalLight*>(this);
  }
  const void *super = nullptr;
  super = iDirectionalLight::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

