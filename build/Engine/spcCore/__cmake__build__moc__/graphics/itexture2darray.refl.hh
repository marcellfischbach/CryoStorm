#pragma once

namespace spc
{

class iTexture2DArrayClass : public spc::Class
{
public:
 iTexture2DArrayClass();
 static iTexture2DArrayClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

