
#pragma once

#include <spcCore/class.hh>
#include <spcCore/types.hh>


namespace spc
{


SPC_CLASS()
class GL4RenderMesh : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMesh();
  virtual ~GL4RenderMesh();


private:

  UInt32 m_vao;

};

}