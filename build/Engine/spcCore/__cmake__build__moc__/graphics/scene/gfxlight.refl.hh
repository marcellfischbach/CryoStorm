#pragma once

namespace spc
{

class GfxLightClass : public spc::Class
{
public:
 GfxLightClass();
 static GfxLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

