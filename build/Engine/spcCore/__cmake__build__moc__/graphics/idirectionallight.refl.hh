#pragma once

namespace spc
{

class iDirectionalLightClass : public spc::Class
{
public:
 iDirectionalLightClass();
 static iDirectionalLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

