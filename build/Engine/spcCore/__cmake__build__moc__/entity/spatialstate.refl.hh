#pragma once

namespace spc
{

class SpatialStateClass : public spc::Class
{
public:
 SpatialStateClass();
 static SpatialStateClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

