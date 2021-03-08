#pragma once

namespace spc
{

class iTextureCubeClass : public spc::Class
{
public:
 iTextureCubeClass();
 static iTextureCubeClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

