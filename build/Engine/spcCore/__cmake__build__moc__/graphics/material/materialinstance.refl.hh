#pragma once

namespace spc
{

class MaterialInstanceClass : public spc::Class
{
public:
 MaterialInstanceClass();
 static MaterialInstanceClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

