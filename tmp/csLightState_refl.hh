#pragma once

namespace cs
{

class csCameraStateClass : public cs::csClass
{
public:
 csCameraStateClass();
 static csCameraStateClass *Get();
 virtual cs::iObject *CreateInstance() const;
};
}

