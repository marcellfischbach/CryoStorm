#pragma once

namespace spc
{

class iRenderTargetClass : public spc::Class
{
public:
 iRenderTargetClass();
 static iRenderTargetClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

