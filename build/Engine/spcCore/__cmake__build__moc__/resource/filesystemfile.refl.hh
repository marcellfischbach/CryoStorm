#pragma once

namespace spc
{

class FileSystemFileClass : public spc::Class
{
public:
 FileSystemFileClass();
 static FileSystemFileClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

