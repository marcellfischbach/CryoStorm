spc::iVertexBufferClass *spc::iVertexBufferClass::Get()
{
  static spc::iVertexBufferClass static_class;
  return &static_class;
};

spc::iVertexBufferClass::iVertexBufferClass()
  : spc::Class("spc::iVertexBuffer")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iVertexBufferClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iVertexBuffer");
}

const spc::Class *spc::iVertexBuffer::GetClass() const
{
  return spc::iVertexBufferClass::Get();
}

const spc::Class *spc::iVertexBuffer::GetStaticClass()
{
  return spc::iVertexBufferClass::Get();
}

void *spc::iVertexBuffer::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iVertexBufferClass::Get())
  {
    return static_cast<spc::iVertexBuffer*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iVertexBuffer::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iVertexBufferClass::Get())
  {
    return static_cast<const spc::iVertexBuffer*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

