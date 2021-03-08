#pragma once

namespace spc::opengl
{

class GL4ShaderClass : public spc::Class
{
public:
 GL4ShaderClass();
 static GL4ShaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

