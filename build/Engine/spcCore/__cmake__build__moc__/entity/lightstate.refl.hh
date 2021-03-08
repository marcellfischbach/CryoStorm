#pragma once

namespace spc
{

class LightStateClass : public spc::Class
{
public:
 LightStateClass();
 static LightStateClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

