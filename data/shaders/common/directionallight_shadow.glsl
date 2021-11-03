

uniform vec4 spc_DirectionalLightShadowMapLayersBias[4];
uniform mat4 spc_DirectionalLightShadowMapMatrices[12];
uniform sampler2DArrayShadow spc_DirectionalLightShadowMapDepth[4];



float calc_directional_shadow(int idx, vec3 light_direction, vec3 frag_position, vec3 camera_space_position)
{
    if (spc_LightCastShadow[idx] == 0)
    {
        return 1.0;
    }


	vec4 layerBias = spc_DirectionalLightShadowMapLayersBias[idx];

	float fadeOut = 0.0f;
	float layer = 0.0;
	int matIndex = 0;
	if (camera_space_position.z <= layerBias.x)
	{
		matIndex = idx * 3;
		layer = 0.0;
	}
	else if (camera_space_position.z <= layerBias.y)
	{
		matIndex = idx * 3 + 1;
		layer = 1.0;
	}
	else if (camera_space_position.z <= layerBias.z)
	{
		matIndex = idx * 3 + 2;
		layer = 2.0;

		fadeOut = smoothstep(layerBias.z - (layerBias.z - layerBias.y) * 0.1, layerBias.z, camera_space_position.z);
	}
	else
	{
		return 1.0;
	}

	vec4 camSpace = spc_DirectionalLightShadowMapMatrices[matIndex] * vec4(frag_position, 1.0);
	camSpace /= camSpace.w;
	camSpace = camSpace * 0.5 + 0.5;
	camSpace.z -= layerBias.w;

	if (camSpace.x < 0.0 || camSpace.x > 1.0 
	|| camSpace.y < 0.0 || camSpace.y > 1.0) 
	{
		return 1.0;
	}



	float v = mix(
		texture(spc_DirectionalLightShadowMapDepth[0], vec4(camSpace.xy, layer, camSpace.z)),
		1.0,
		fadeOut);
	return v; //* fact;
}
