#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/math/clipper.hh>
#include <SpiceCore/math/plane.hh>
#include <vector>


namespace Spice {



class SPICE_CORE_API PlaneClipper : public Clipper
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

