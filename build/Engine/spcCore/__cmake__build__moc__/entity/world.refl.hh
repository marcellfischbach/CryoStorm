#pragma once

namespace spc
{

class WorldClass : public spc::Class
{
public:
 WorldClass();
 static WorldClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

