#pragma once

namespace spc
{

class SDLInputSystemClass : public spc::Class
{
public:
 SDLInputSystemClass();
 static SDLInputSystemClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

