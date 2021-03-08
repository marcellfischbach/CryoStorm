#pragma once

namespace spc
{

class ObjectClass : public spc::Class
{
public:
 ObjectClass();
 static ObjectClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

