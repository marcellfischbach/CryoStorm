#pragma once

namespace spc::img
{

class PngLoaderClass : public spc::Class
{
public:
 PngLoaderClass();
 static PngLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

