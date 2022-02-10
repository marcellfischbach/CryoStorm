
#pragma once


namespace ce
{

enum eRenderPass
{
  eRP_Depth,
  eRP_GBuffer,
  eRP_Forward,
  eRP_Shadow,
  eRP_ShadowPSSM,
  eRP_ShadowCube,

  eRP_COUNT
};

}