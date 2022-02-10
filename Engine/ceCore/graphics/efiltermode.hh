

#pragma once

namespace ce
{


enum eFilterMode
{
  eFM_MinMagNearest,
  eFM_MinNearestMagLinear,
  eFM_MinLinearMagNearest,
  eFM_MinMagLinear,
  eFM_MinMagMipNearest,
  eFM_MinMagNearestMipLinear,
  eFM_MinNearestMagLinearMipNearest,
  eFM_MinNearestMagMipLinear,
  eFM_MinLinearMagMipNearest,
  eFM_MinLinearMagNearestMipLinear,
  eFM_MinMagLinearMipNearest,
  eFM_MinMagMipLinear,
  eFM_Anisotropic,
};

}