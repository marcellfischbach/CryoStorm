

uniform vec4 spc_DirectionalLightShadowMapLayersBias[4];
uniform mat4 spc_DirectionalLightShadowMapMatrices[12];
uniform sampler2DArrayShadow spc_DirectionalLightShadowMapDepth[2];



float calc_directional_shadow(int idx, vec3 light_direction, vec3 frag_position, vec3 camera_space_position)
{
    if (spc_LightCastShadow[idx] == 0)
    {
        return 1.0;
    }


	vec4 layerBias = spc_DirectionalLightShadowMapLayersBias[idx];

	float layer = 0.0;
	int matIndex = 0;
	if (camera_space_position.z <= layerBias.x)
	{
		matIndex = idx * 3;
	}
	else if (camera_space_position.z <= layerBias.y)
	{
		matIndex = idx * 3 + 1;
	}
	else if (camera_space_position.z <= layerBias.z)
	{
		matIndex = idx * 3 + 2;
	}
	else
	{
		matIndex = idx * 3 + 2;
	}

	vec4 camSpace = spc_DirectionalLightShadowMapMatrices[matIndex] * vec4(frag_position, 1.0);
	camSpace /= camSpace.w;
	camSpace = camSpace * 0.5 + 0.5;
	camSpace.z -= layerBias.w;



	float v = texture(spc_DirectionalLightShadowMapDepth[0], vec4(camSpace.xy, layer, camSpace.z));
	return v;
}
