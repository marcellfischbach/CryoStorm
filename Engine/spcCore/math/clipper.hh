#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/math/vector3f.hh>
#include <vector>



namespace spc {


class SPC_CORE_API Clipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~Clipper();

  virtual ClipResult Test(const Vector3f& pos) const = 0;
  virtual ClipResult Test(const BoundingBox& bbox, bool debug = false) const = 0;


protected:
  Clipper();


};

}
