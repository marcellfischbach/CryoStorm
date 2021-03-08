#pragma once

namespace spc::opengl
{

class GL4PointLightClass : public spc::Class
{
public:
 GL4PointLightClass();
 static GL4PointLightClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

