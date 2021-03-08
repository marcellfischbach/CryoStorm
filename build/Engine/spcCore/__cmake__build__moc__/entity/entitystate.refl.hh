#pragma once

namespace spc
{

class EntityStateClass : public spc::Class
{
public:
 EntityStateClass();
 static EntityStateClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

