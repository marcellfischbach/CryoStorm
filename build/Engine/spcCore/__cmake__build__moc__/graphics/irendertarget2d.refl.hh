#pragma once

namespace spc
{

class iRenderTarget2DClass : public spc::Class
{
public:
 iRenderTarget2DClass();
 static iRenderTarget2DClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

