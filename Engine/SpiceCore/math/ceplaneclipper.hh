#pragma once

#include <cecore/ceexport.hh>
#include <cecore/math/ceclipper.hh>
#include <cecore/math/ceplane.hh>
#include <vector>


class CEC_API cePlaneClipper : public ceClipper
{
public:
  cePlaneClipper();
  virtual ~cePlaneClipper();

  void Clear();

  virtual ClipResult Test(const ceVector3f& pos) const;
  virtual ClipResult Test(const ceBoundingBox& bbox, bool debug = false) const;

  void AddPlane(const cePlane& plane);

private:
  std::vector<cePlane> m_planes;

};


