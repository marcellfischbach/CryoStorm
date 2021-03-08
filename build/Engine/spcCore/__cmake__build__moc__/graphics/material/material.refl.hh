#pragma once

namespace spc
{

class MaterialClass : public spc::Class
{
public:
 MaterialClass();
 static MaterialClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

