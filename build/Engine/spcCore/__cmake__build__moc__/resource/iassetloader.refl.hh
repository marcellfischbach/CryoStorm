#pragma once

namespace spc
{

class iAssetLoaderClass : public spc::Class
{
public:
 iAssetLoaderClass();
 static iAssetLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc
{

class iAssetLoaderSpcClass : public spc::Class
{
public:
 iAssetLoaderSpcClass();
 static iAssetLoaderSpcClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

