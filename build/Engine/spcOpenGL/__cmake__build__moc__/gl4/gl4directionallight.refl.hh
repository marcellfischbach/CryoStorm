#pragma once

namespace spc::opengl
{

class GL4DirectionalLightClass : public spc::Class
{
public:
 GL4DirectionalLightClass();
 static GL4DirectionalLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

