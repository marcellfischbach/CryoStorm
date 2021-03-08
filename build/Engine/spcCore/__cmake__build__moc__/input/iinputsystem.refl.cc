spc::iInputSystemClass *spc::iInputSystemClass::Get()
{
  static spc::iInputSystemClass static_class;
  return &static_class;
};

spc::iInputSystemClass::iInputSystemClass()
  : spc::Class("spc::iInputSystem")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iInputSystemClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iInputSystem");
}

const spc::Class *spc::iInputSystem::GetClass() const
{
  return spc::iInputSystemClass::Get();
}

const spc::Class *spc::iInputSystem::GetStaticClass()
{
  return spc::iInputSystemClass::Get();
}

void *spc::iInputSystem::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iInputSystemClass::Get())
  {
    return static_cast<spc::iInputSystem*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iInputSystem::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iInputSystemClass::Get())
  {
    return static_cast<const spc::iInputSystem*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

