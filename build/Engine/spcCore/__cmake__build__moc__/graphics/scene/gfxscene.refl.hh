#pragma once

namespace spc
{

class GfxSceneClass : public spc::Class
{
public:
 GfxSceneClass();
 static GfxSceneClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

