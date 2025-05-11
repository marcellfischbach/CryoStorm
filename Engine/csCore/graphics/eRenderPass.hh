
#pragma once


namespace cs
{

enum eRenderPass
{
  eRP_Depth,
  eRP_GBuffer,
  eRP_Forward,
  eRP_Shadow,
  eRP_VSM,
  eRP_DeltaSM,
  eRP_ShadowPSSM,
  eRP_ShadowCube,

  eRP_COUNT
};

}