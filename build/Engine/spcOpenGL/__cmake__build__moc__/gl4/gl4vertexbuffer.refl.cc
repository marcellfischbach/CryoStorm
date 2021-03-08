spc::opengl::GL4VertexBufferClass *spc::opengl::GL4VertexBufferClass::Get()
{
  static spc::opengl::GL4VertexBufferClass static_class;
  return &static_class;
};

spc::opengl::GL4VertexBufferClass::GL4VertexBufferClass()
  : spc::Class("spc::opengl::GL4VertexBuffer")
{
  AddSuperClass(iVertexBuffer::GetStaticClass());
}

spc::iObject *spc::opengl::GL4VertexBufferClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4VertexBuffer());
}

const spc::Class *spc::opengl::GL4VertexBuffer::GetClass() const
{
  return spc::opengl::GL4VertexBufferClass::Get();
}

const spc::Class *spc::opengl::GL4VertexBuffer::GetStaticClass()
{
  return spc::opengl::GL4VertexBufferClass::Get();
}

void *spc::opengl::GL4VertexBuffer::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4VertexBufferClass::Get())
  {
    return static_cast<spc::opengl::GL4VertexBuffer*>(this);
  }
  void *super = nullptr;
  super = iVertexBuffer::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4VertexBuffer::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4VertexBufferClass::Get())
  {
    return static_cast<const spc::opengl::GL4VertexBuffer*>(this);
  }
  const void *super = nullptr;
  super = iVertexBuffer::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

