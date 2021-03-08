#pragma once

namespace spc::opengl
{

class GL4DeviceClass : public spc::Class
{
public:
 GL4DeviceClass();
 static GL4DeviceClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

