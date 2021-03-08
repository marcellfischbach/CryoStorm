#pragma once

namespace spc
{

class StaticMeshStateClass : public spc::Class
{
public:
 StaticMeshStateClass();
 static StaticMeshStateClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

