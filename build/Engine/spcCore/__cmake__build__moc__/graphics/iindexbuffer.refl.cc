spc::iIndexBufferClass *spc::iIndexBufferClass::Get()
{
  static spc::iIndexBufferClass static_class;
  return &static_class;
};

spc::iIndexBufferClass::iIndexBufferClass()
  : spc::Class("spc::iIndexBuffer")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iIndexBufferClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iIndexBuffer");
}

const spc::Class *spc::iIndexBuffer::GetClass() const
{
  return spc::iIndexBufferClass::Get();
}

const spc::Class *spc::iIndexBuffer::GetStaticClass()
{
  return spc::iIndexBufferClass::Get();
}

void *spc::iIndexBuffer::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iIndexBufferClass::Get())
  {
    return static_cast<spc::iIndexBuffer*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iIndexBuffer::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iIndexBufferClass::Get())
  {
    return static_cast<const spc::iIndexBuffer*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

