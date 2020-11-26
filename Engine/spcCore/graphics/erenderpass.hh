
#pragma once


namespace spc
{

enum eRenderPass
{
  eRP_Depth,
  eRP_GBuffer,
  eRP_Forward,
  eRP_ShadowPSSM,
  eRP_ShadowCube,

  eRP_COUNT
};

}