#pragma once

namespace spc
{

class iSamplerClass : public spc::Class
{
public:
 iSamplerClass();
 static iSamplerClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

