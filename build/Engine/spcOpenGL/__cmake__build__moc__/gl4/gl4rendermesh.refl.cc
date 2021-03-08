spc::opengl::GL4RenderMeshClass *spc::opengl::GL4RenderMeshClass::Get()
{
  static spc::opengl::GL4RenderMeshClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderMeshClass::GL4RenderMeshClass()
  : spc::Class("spc::opengl::GL4RenderMesh")
{
  AddSuperClass(iRenderMesh::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderMeshClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::opengl::GL4RenderMesh");
}

const spc::Class *spc::opengl::GL4RenderMesh::GetClass() const
{
  return spc::opengl::GL4RenderMeshClass::Get();
}

const spc::Class *spc::opengl::GL4RenderMesh::GetStaticClass()
{
  return spc::opengl::GL4RenderMeshClass::Get();
}

void *spc::opengl::GL4RenderMesh::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderMeshClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderMesh*>(this);
  }
  void *super = nullptr;
  super = iRenderMesh::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderMesh::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderMeshClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderMesh*>(this);
  }
  const void *super = nullptr;
  super = iRenderMesh::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::opengl::GL4RenderMeshGeneratorClass *spc::opengl::GL4RenderMeshGeneratorClass::Get()
{
  static spc::opengl::GL4RenderMeshGeneratorClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderMeshGeneratorClass::GL4RenderMeshGeneratorClass()
  : spc::Class("spc::opengl::GL4RenderMeshGenerator")
{
  AddSuperClass(iRenderMeshGenerator::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderMeshGeneratorClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4RenderMeshGenerator());
}

const spc::Class *spc::opengl::GL4RenderMeshGenerator::GetClass() const
{
  return spc::opengl::GL4RenderMeshGeneratorClass::Get();
}

const spc::Class *spc::opengl::GL4RenderMeshGenerator::GetStaticClass()
{
  return spc::opengl::GL4RenderMeshGeneratorClass::Get();
}

void *spc::opengl::GL4RenderMeshGenerator::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderMeshGeneratorClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderMeshGenerator*>(this);
  }
  void *super = nullptr;
  super = iRenderMeshGenerator::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderMeshGenerator::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderMeshGeneratorClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderMeshGenerator*>(this);
  }
  const void *super = nullptr;
  super = iRenderMeshGenerator::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

spc::opengl::GL4RenderMeshGeneratorFactoryClass *spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get()
{
  static spc::opengl::GL4RenderMeshGeneratorFactoryClass static_class;
  return &static_class;
};

spc::opengl::GL4RenderMeshGeneratorFactoryClass::GL4RenderMeshGeneratorFactoryClass()
  : spc::Class("spc::opengl::GL4RenderMeshGeneratorFactory")
{
  AddSuperClass(iRenderMeshGeneratorFactory::GetStaticClass());
}

spc::iObject *spc::opengl::GL4RenderMeshGeneratorFactoryClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4RenderMeshGeneratorFactory());
}

const spc::Class *spc::opengl::GL4RenderMeshGeneratorFactory::GetClass() const
{
  return spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get();
}

const spc::Class *spc::opengl::GL4RenderMeshGeneratorFactory::GetStaticClass()
{
  return spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get();
}

void *spc::opengl::GL4RenderMeshGeneratorFactory::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get())
  {
    return static_cast<spc::opengl::GL4RenderMeshGeneratorFactory*>(this);
  }
  void *super = nullptr;
  super = iRenderMeshGeneratorFactory::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4RenderMeshGeneratorFactory::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4RenderMeshGeneratorFactoryClass::Get())
  {
    return static_cast<const spc::opengl::GL4RenderMeshGeneratorFactory*>(this);
  }
  const void *super = nullptr;
  super = iRenderMeshGeneratorFactory::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

