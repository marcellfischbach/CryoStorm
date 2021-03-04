

uniform vec4 spc_DirectionalLightShadowMapLayersBias[4];
uniform mat4 spc_DirectionalLightShadowMapMatrices[12];
uniform sampler2DArray spc_DirectionalLightShadowMapColor[4];
uniform sampler2DArray spc_DirectionalLightShadowMapDepth[4];



float calc_directional_shadow(int idx, vec3 light_direction, vec3 frag_position, vec3 camera_space_position)
{
	if (camera_space_position.z > 10.0)
	{
		return 0.5;
	}
	return 1.0;
}
