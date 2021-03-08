#pragma once

namespace spc
{

class iPointLightClass : public spc::Class
{
public:
 iPointLightClass();
 static iPointLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

