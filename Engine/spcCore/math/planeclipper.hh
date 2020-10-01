#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/math/clipper.hh>
#include <spcCore/math/plane.hh>
#include <vector>


namespace spc {



class SPC_CORE_API PlaneClipper : public Clipper
{
public:
  PlaneClipper();
  virtual ~PlaneClipper();

  void Clear();

  virtual ClipResult Test(const Vector3f& pos) const;
  virtual ClipResult Test(const BoundingBox& bbox, bool debug = false) const;

  void AddPlane(const Plane& plane);

private:
  std::vector<Plane> m_planes;

};



}

