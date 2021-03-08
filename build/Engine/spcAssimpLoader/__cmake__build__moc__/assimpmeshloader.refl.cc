spc::assimp::AssimpMeshLoaderClass *spc::assimp::AssimpMeshLoaderClass::Get()
{
  static spc::assimp::AssimpMeshLoaderClass static_class;
  return &static_class;
};

spc::assimp::AssimpMeshLoaderClass::AssimpMeshLoaderClass()
  : spc::Class("spc::assimp::AssimpMeshLoader")
{
  AddSuperClass(iAssetLoader::GetStaticClass());
}

spc::iObject *spc::assimp::AssimpMeshLoaderClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::assimp::AssimpMeshLoader());
}

const spc::Class *spc::assimp::AssimpMeshLoader::GetClass() const
{
  return spc::assimp::AssimpMeshLoaderClass::Get();
}

const spc::Class *spc::assimp::AssimpMeshLoader::GetStaticClass()
{
  return spc::assimp::AssimpMeshLoaderClass::Get();
}

void *spc::assimp::AssimpMeshLoader::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::assimp::AssimpMeshLoaderClass::Get())
  {
    return static_cast<spc::assimp::AssimpMeshLoader*>(this);
  }
  void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::assimp::AssimpMeshLoader::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::assimp::AssimpMeshLoaderClass::Get())
  {
    return static_cast<const spc::assimp::AssimpMeshLoader*>(this);
  }
  const void *super = nullptr;
  super = iAssetLoader::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

