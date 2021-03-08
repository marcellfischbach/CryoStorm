#pragma once

namespace spc
{

class iRenderPipelineClass : public spc::Class
{
public:
 iRenderPipelineClass();
 static iRenderPipelineClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

