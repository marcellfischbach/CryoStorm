#pragma once

namespace spc
{

class iLightClass : public spc::Class
{
public:
 iLightClass();
 static iLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

