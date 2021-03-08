#pragma once

namespace spc
{

class iTextureClass : public spc::Class
{
public:
 iTextureClass();
 static iTextureClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

