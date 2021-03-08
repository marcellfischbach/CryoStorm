spc::MeshClass *spc::MeshClass::Get()
{
  static spc::MeshClass static_class;
  return &static_class;
};

spc::MeshClass::MeshClass()
  : spc::Class("spc::Mesh")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::MeshClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::Mesh());
}

const spc::Class *spc::Mesh::GetClass() const
{
  return spc::MeshClass::Get();
}

const spc::Class *spc::Mesh::GetStaticClass()
{
  return spc::MeshClass::Get();
}

void *spc::Mesh::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::MeshClass::Get())
  {
    return static_cast<spc::Mesh*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::Mesh::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::MeshClass::Get())
  {
    return static_cast<const spc::Mesh*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

