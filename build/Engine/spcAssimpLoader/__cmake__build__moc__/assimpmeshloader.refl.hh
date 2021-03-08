#pragma once

namespace spc::assimp
{

class AssimpMeshLoaderClass : public spc::Class
{
public:
 AssimpMeshLoaderClass();
 static AssimpMeshLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

