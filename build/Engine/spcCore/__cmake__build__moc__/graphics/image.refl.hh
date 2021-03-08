#pragma once

namespace spc
{

class ImageClass : public spc::Class
{
public:
 ImageClass();
 static ImageClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

