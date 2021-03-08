#pragma once

namespace spc
{

class GfxMeshClass : public spc::Class
{
public:
 GfxMeshClass();
 static GfxMeshClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

