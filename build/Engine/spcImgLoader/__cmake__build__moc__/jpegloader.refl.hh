#pragma once

namespace spc::img
{

class JpegLoaderClass : public spc::Class
{
public:
 JpegLoaderClass();
 static JpegLoaderClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

