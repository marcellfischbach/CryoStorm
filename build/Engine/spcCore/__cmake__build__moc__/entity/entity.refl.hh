#pragma once

namespace spc
{

class EntityClass : public spc::Class
{
public:
 EntityClass();
 static EntityClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

