#pragma once

namespace spc
{

class iTexture2DClass : public spc::Class
{
public:
 iTexture2DClass();
 static iTexture2DClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

