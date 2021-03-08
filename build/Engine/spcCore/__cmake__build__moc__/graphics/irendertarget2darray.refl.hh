#pragma once

namespace spc
{

class iRenderTarget2DArrayClass : public spc::Class
{
public:
 iRenderTarget2DArrayClass();
 static iRenderTarget2DArrayClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

