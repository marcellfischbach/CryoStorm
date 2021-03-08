#pragma once

namespace spc
{

class iMaterialClass : public spc::Class
{
public:
 iMaterialClass();
 static iMaterialClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

