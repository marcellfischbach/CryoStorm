spc::SDLInputSystemClass *spc::SDLInputSystemClass::Get()
{
  static spc::SDLInputSystemClass static_class;
  return &static_class;
};

spc::SDLInputSystemClass::SDLInputSystemClass()
  : spc::Class("spc::SDLInputSystem")
{
  AddSuperClass(iInputSystem::GetStaticClass());
}

spc::iObject *spc::SDLInputSystemClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::SDLInputSystem");
}

const spc::Class *spc::SDLInputSystem::GetClass() const
{
  return spc::SDLInputSystemClass::Get();
}

const spc::Class *spc::SDLInputSystem::GetStaticClass()
{
  return spc::SDLInputSystemClass::Get();
}

void *spc::SDLInputSystem::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::SDLInputSystemClass::Get())
  {
    return static_cast<spc::SDLInputSystem*>(this);
  }
  void *super = nullptr;
  super = iInputSystem::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::SDLInputSystem::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::SDLInputSystemClass::Get())
  {
    return static_cast<const spc::SDLInputSystem*>(this);
  }
  const void *super = nullptr;
  super = iInputSystem::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

