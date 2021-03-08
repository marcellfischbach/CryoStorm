#pragma once

namespace spc
{

class CameraClass : public spc::Class
{
public:
 CameraClass();
 static CameraClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

