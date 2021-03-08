spc::iRenderMeshClass *spc::iRenderMeshClass::Get()
{
  static spc::iRenderMeshClass static_class;
  return &static_class;
};

spc::iRenderMeshClass::iRenderMeshClass()
  : spc::Class("spc::iRenderMesh")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iRenderMeshClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderMesh");
}

const spc::Class *spc::iRenderMesh::GetClass() const
{
  return spc::iRenderMeshClass::Get();
}

const spc::Class *spc::iRenderMesh::GetStaticClass()
{
  return spc::iRenderMeshClass::Get();
}

void *spc::iRenderMesh::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderMeshClass::Get())
  {
    return static_cast<spc::iRenderMesh*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderMesh::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderMeshClass::Get())
  {
    return static_cast<const spc::iRenderMesh*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::iRenderMeshGeneratorClass *spc::iRenderMeshGeneratorClass::Get()
{
  static spc::iRenderMeshGeneratorClass static_class;
  return &static_class;
};

spc::iRenderMeshGeneratorClass::iRenderMeshGeneratorClass()
  : spc::Class("spc::iRenderMeshGenerator")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iRenderMeshGeneratorClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderMeshGenerator");
}

const spc::Class *spc::iRenderMeshGenerator::GetClass() const
{
  return spc::iRenderMeshGeneratorClass::Get();
}

const spc::Class *spc::iRenderMeshGenerator::GetStaticClass()
{
  return spc::iRenderMeshGeneratorClass::Get();
}

void *spc::iRenderMeshGenerator::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderMeshGeneratorClass::Get())
  {
    return static_cast<spc::iRenderMeshGenerator*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderMeshGenerator::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderMeshGeneratorClass::Get())
  {
    return static_cast<const spc::iRenderMeshGenerator*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::iRenderMeshGeneratorFactoryClass *spc::iRenderMeshGeneratorFactoryClass::Get()
{
  static spc::iRenderMeshGeneratorFactoryClass static_class;
  return &static_class;
};

spc::iRenderMeshGeneratorFactoryClass::iRenderMeshGeneratorFactoryClass()
  : spc::Class("spc::iRenderMeshGeneratorFactory")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iRenderMeshGeneratorFactoryClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderMeshGeneratorFactory");
}

const spc::Class *spc::iRenderMeshGeneratorFactory::GetClass() const
{
  return spc::iRenderMeshGeneratorFactoryClass::Get();
}

const spc::Class *spc::iRenderMeshGeneratorFactory::GetStaticClass()
{
  return spc::iRenderMeshGeneratorFactoryClass::Get();
}

void *spc::iRenderMeshGeneratorFactory::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderMeshGeneratorFactoryClass::Get())
  {
    return static_cast<spc::iRenderMeshGeneratorFactory*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderMeshGeneratorFactory::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderMeshGeneratorFactoryClass::Get())
  {
    return static_cast<const spc::iRenderMeshGeneratorFactory*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

