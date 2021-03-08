spc::GfxMeshClass *spc::GfxMeshClass::Get()
{
  static spc::GfxMeshClass static_class;
  return &static_class;
};

spc::GfxMeshClass::GfxMeshClass()
  : spc::Class("spc::GfxMesh")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::GfxMeshClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::GfxMesh());
}

const spc::Class *spc::GfxMesh::GetClass() const
{
  return spc::GfxMeshClass::Get();
}

const spc::Class *spc::GfxMesh::GetStaticClass()
{
  return spc::GfxMeshClass::Get();
}

void *spc::GfxMesh::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::GfxMeshClass::Get())
  {
    return static_cast<spc::GfxMesh*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::GfxMesh::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::GfxMeshClass::Get())
  {
    return static_cast<const spc::GfxMesh*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

