#pragma once

namespace spc::opengl
{

class GL4TextureCubeClass : public spc::Class
{
public:
 GL4TextureCubeClass();
 static GL4TextureCubeClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

