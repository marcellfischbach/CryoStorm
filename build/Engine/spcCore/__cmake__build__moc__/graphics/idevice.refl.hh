#pragma once

namespace spc
{

class iDeviceClass : public spc::Class
{
public:
 iDeviceClass();
 static iDeviceClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

