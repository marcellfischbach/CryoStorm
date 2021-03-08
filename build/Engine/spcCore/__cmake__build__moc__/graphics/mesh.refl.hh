#pragma once

namespace spc
{

class MeshClass : public spc::Class
{
public:
 MeshClass();
 static MeshClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

