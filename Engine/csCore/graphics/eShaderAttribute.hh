
#pragma once

namespace cs
{


enum eShaderAttribute
{
  /* Matrices */
  eSA_ModelMatrix,
  eSA_ViewMatrix,
  eSA_ProjectionMatrix,
  eSA_ModelViewMatrix,
  eSA_ViewProjectionMatrix,
  eSA_ModelViewProjectionMatrix,
  eSA_ModelMatrixInv,
  eSA_ViewMatrixInv,
  eSA_ProjectionMatrixInv,
  eSA_ModelViewMatrixInv,
  eSA_ViewProjectionMatrixInv,
  eSA_ModelViewProjectionMatrixInv,

  eSA_Random,

  eSA_ShadowMapViewMatrix,
  eSA_ShadowMapProjectionMatrix,
  eSA_ShadowMapViewProjectionMatrix,

  eSA_ShadowMapViewMatrixInv,
  eSA_ShadowMapProjectionMatrixInv,
  eSA_ShadowMapViewProjectionMatrixInv,

  eSA_RenderLayer,

  /* Lighting */
  eSA_LightColor,
  eSA_LightVector,
  eSA_LightRange,
  eSA_LightCount,
  eSA_LightCastShadow,
  eSA_LightShadowMap,

  /* ShadowMapping */
  eSA_PointLightShadowMapMappingBias,
  eSA_PointLightShadowMapColor,
  eSA_PointLightShadowMapDepth,



  eSA_DirectionalLightShadowMapSplitLayers,
  eSA_DirectionalLightShadowMapViewProjectionMatrix,
  eSA_DirectionalLightShadowMapDepth,
  eSA_DirectionalLightShadowMapColor,
  eSA_DirectionalLightShadowMapLayersBias,


  /* Skeleton rendering */
  eSA_SkeletonMatrices,
};

}