#pragma once

namespace spc::opengl
{

class GL4ShaderSourceFragmentClass : public spc::Class
{
public:
 GL4ShaderSourceFragmentClass();
 static GL4ShaderSourceFragmentClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

