#pragma once

namespace cs
{

class csSpatialStateClass : public cs::csClass
{
public:
 csSpatialStateClass();
 static csSpatialStateClass *Get();
 virtual cs::iObject *CreateInstance() const;
};
}

