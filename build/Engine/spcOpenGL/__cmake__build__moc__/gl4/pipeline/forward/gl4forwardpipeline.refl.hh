#pragma once

namespace spc
{
namespace opengl
{

class GL4ForwardPipelineClass : public spc::Class
{
public:
 GL4ForwardPipelineClass();
 static GL4ForwardPipelineClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}
}

