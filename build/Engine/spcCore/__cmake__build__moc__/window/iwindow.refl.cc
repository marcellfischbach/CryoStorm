spc::iWindowClass *spc::iWindowClass::Get()
{
  static spc::iWindowClass static_class;
  return &static_class;
};

spc::iWindowClass::iWindowClass()
  : spc::Class("spc::iWindow")
{
}

spc::iObject *spc::iWindowClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iWindow");
}

const spc::Class *spc::iWindow::GetClass() const
{
  return spc::iWindowClass::Get();
}

const spc::Class *spc::iWindow::GetStaticClass()
{
  return spc::iWindowClass::Get();
}

void *spc::iWindow::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iWindowClass::Get())
  {
    return static_cast<spc::iWindow*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iWindow::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iWindowClass::Get())
  {
    return static_cast<const spc::iWindow*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

