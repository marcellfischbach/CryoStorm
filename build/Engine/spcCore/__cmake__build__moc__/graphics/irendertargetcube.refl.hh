#pragma once

namespace spc
{

class iRenderTargetCubeClass : public spc::Class
{
public:
 iRenderTargetCubeClass();
 static iRenderTargetCubeClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

