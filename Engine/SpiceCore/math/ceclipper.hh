#pragma once

#include <cecore/ceexport.hh>
#include <cecore/math/ceboundingbox.hh>
#include <cecore/math/cevector3f.hh>
#include <vector>


class CEC_API ceClipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~ceClipper();

  virtual ClipResult Test(const ceVector3f& pos) const = 0;
  virtual ClipResult Test(const ceBoundingBox& bbox, bool debug = false) const = 0;


protected:
  ceClipper();


};

