

uniform float ce_DirectionalLightShadowMapLayersDepth;
uniform float ce_DirectionalLightShadowMapLayersBias;
uniform mat4 ce_DirectionalLightShadowMapMatrices[3];
uniform sampler2DArrayShadow ce_DirectionalLightShadowMapDepth;



float calc_directional_shadow(int idx, vec3 light_direction, vec3 frag_position, vec3 camera_space_position)
{
    if (ce_LightCastShadow[idx] == 0)
    {
        return 1.0;
    }


	vec4 layerDepth = ce_DirectionalLightShadowMapLayersDepth[idx];

	float fadeOut = 0.0f;
	float layer = 0.0;
	int matIndex = 0;
	if (camera_space_position.z <= layerDepth.x)
	{
		matIndex = idx * 3;
		layer = 0.0;
	}
	else if (camera_space_position.z <= layerDepth.y)
	{
		matIndex = idx * 3 + 1;
		layer = 1.0;
	}
	else if (camera_space_position.z <= layerDepth.z)
	{
		matIndex = idx * 3 + 2;
		layer = 2.0;

		fadeOut = smoothstep(layerDepth.z - (layerDepth.z - layerDepth.y) * 0.1, layerDepth.z, camera_space_position.z);
	}
	else
	{
		//return 1.0;
	}

	vec4 camSpace = ce_DirectionalLightShadowMapMatrices[matIndex] * vec4(frag_position, 1.0);
	camSpace /= camSpace.w;
	camSpace = camSpace * 0.5 + 0.5;
	camSpace.z -= ce_DirectionalLightShadowMapLayersBias;

//	if (camSpace.x < 0.0 || camSpace.x > 1.0
//	|| camSpace.y < 0.0 || camSpace.y > 1.0)
//	{
//		return 1.0;
//	}



	return mix(
		texture(ce_DirectionalLightShadowMapDepth[idx], vec4(camSpace.xy, layer, camSpace.z)),
		1.0,
		fadeOut);
}
