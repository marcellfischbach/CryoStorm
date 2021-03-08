spc::opengl::GL4IndexBufferClass *spc::opengl::GL4IndexBufferClass::Get()
{
  static spc::opengl::GL4IndexBufferClass static_class;
  return &static_class;
};

spc::opengl::GL4IndexBufferClass::GL4IndexBufferClass()
  : spc::Class("spc::opengl::GL4IndexBuffer")
{
  AddSuperClass(iIndexBuffer::GetStaticClass());
}

spc::iObject *spc::opengl::GL4IndexBufferClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4IndexBuffer());
}

const spc::Class *spc::opengl::GL4IndexBuffer::GetClass() const
{
  return spc::opengl::GL4IndexBufferClass::Get();
}

const spc::Class *spc::opengl::GL4IndexBuffer::GetStaticClass()
{
  return spc::opengl::GL4IndexBufferClass::Get();
}

void *spc::opengl::GL4IndexBuffer::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4IndexBufferClass::Get())
  {
    return static_cast<spc::opengl::GL4IndexBuffer*>(this);
  }
  void *super = nullptr;
  super = iIndexBuffer::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4IndexBuffer::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4IndexBufferClass::Get())
  {
    return static_cast<const spc::opengl::GL4IndexBuffer*>(this);
  }
  const void *super = nullptr;
  super = iIndexBuffer::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

