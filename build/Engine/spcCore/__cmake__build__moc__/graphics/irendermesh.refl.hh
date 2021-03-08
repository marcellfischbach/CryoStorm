#pragma once

namespace spc
{

class iRenderMeshClass : public spc::Class
{
public:
 iRenderMeshClass();
 static iRenderMeshClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc
{

class iRenderMeshGeneratorClass : public spc::Class
{
public:
 iRenderMeshGeneratorClass();
 static iRenderMeshGeneratorClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

#pragma once

namespace spc
{

class iRenderMeshGeneratorFactoryClass : public spc::Class
{
public:
 iRenderMeshGeneratorFactoryClass();
 static iRenderMeshGeneratorFactoryClass *Get();
 virtual spc::iObject *CreateInstance() const;
};
}

