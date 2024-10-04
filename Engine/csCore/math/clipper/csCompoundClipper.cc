
#include <csCore/math/clipper/csCompoundClipper.hh>

namespace cryo
{


csCompoundClipper::csCompoundClipper()
  : iClipper()
{

}
eClippingResult csCompoundClipper::Test(const csBoundingBox& bbox) const
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

eClippingResult csCompoundClipper::Test(const csSphere& sphere) const
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

eClippingResult csCompoundClipper::Test(const csVector3f& p) const
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