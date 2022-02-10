
#include <ceCore/math/clipper/compoundclipper.hh>

namespace ce
{


CompoundClipper::CompoundClipper()
  : iClipper()
{

}
eClippingResult CompoundClipper::Test(const BoundingBox& bbox) const
{
  eClippingResult result = eClippingResult::eCR_Inside;
  for (auto clipper : m_clippers)
  {
    eClippingResult res = clipper->Test(bbox);
    switch (res)
    {
      case eClippingResult::eCR_Outside:
        return eClippingResult::eCR_Outside;
      case eClippingResult::eCR_Intermediate:
        result = eClippingResult::eCR_Intermediate;
      default:
        break;
    }
  }
  return result;
}

eClippingResult CompoundClipper::Test(const Sphere& sphere) const
{
  eClippingResult result = eClippingResult::eCR_Inside;
  for (auto clipper : m_clippers)
  {
    eClippingResult res = clipper->Test(sphere);
    switch (res)
    {
    case eClippingResult::eCR_Outside:
      return eClippingResult::eCR_Outside;
    case eClippingResult::eCR_Intermediate:
      result = eClippingResult::eCR_Intermediate;
    default:
      break;
    }
  }
  return result;
}

eClippingResult CompoundClipper::Test(const Vector3f& p) const
{
  for (auto clipper : m_clippers)
  {
    eClippingResult res = clipper->Test(p);
    if (res == eClippingResult::eCR_Outside)
    {
      return eClippingResult::eCR_Outside;
    }
  }
  return eClippingResult::eCR_Inside;

}


}