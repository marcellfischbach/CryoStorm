#pragma once

namespace spc
{

class iShaderClass : public spc::Class
{
public:
 iShaderClass();
 static iShaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

