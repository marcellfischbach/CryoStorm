#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DepthBuffer;
uniform sampler2D ce_ShadowMap;
uniform mat4 ce_ProjectionMatrixInv;
uniform float ce_FilterRadius;
uniform float ce_FilterSamplesFactor;
uniform vec2 ce_FilterDistance;
uniform float ce_FilterMaxSampleDistance;
uniform vec2 ce_ScreenSize;
uniform float ce_ScreenAspect;

in vec2 texCoord;



vec3 get_position_in_camera_space(vec2 uv)
{
	float depth = texture(ce_DepthBuffer, uv).r;
	if (depth == 1.0)
	{
		return vec3(0, 0, -1);
	}
	vec4 proj_space = vec4(uv.xy, depth, 1.0);
	proj_space = proj_space * 2.0 - 1.0;
	vec4 cam_space = ce_ProjectionMatrixInv * proj_space;
	return cam_space.xyz / cam_space.w;

}


void main ()
{
	vec3 position_in_camera_space = get_position_in_camera_space(texCoord);
	if (position_in_camera_space == vec3(0, 0, -1))
	{
		ce_FragColor = vec4(1, 1, 1, 1);
		return;
	}
	float distance_to_camera = position_in_camera_space.z;

	float distance_factor = clamp((distance_to_camera - ce_FilterDistance.x) / ce_FilterDistance.y, 0.0, 1.0);
	float filter_radius = mix(ce_FilterRadius, 0.0, distance_factor);
	// filter radius is the depth cleaned pixel radius for sampling

	int sizeX = int(filter_radius * ce_ScreenSize.x * ce_FilterSamplesFactor / 2.0);
	int sizeY = int(filter_radius * ce_ScreenSize.y * ce_FilterSamplesFactor / 2.0);

	if (sizeX == 0 || sizeY == 0)
	{
		ce_FragColor = texture(ce_ShadowMap, texCoord);
	}
	else
	{

		vec4 color;
		float strength = 0.0;
		for (int i=-sizeX; i<=sizeX; i++)
		{
			float x = float(i) / float(sizeX);
			float sampleX = x * filter_radius;
			for (int j=-sizeY; j<=sizeY; j++)
			{
				float y = float(j) / float(sizeY);
				float sampleY = y * filter_radius * ce_ScreenAspect;

				vec2 tc = texCoord + vec2(sampleX, sampleY);
				vec3 sample_pos = get_position_in_camera_space(tc);

				float distance_to_reference = length(sample_pos - position_in_camera_space);
				float sampleStrength = max(1.0 - (distance_to_reference / ce_FilterMaxSampleDistance), 0.0);

				if (sampleStrength != 0.0)
				{

					color += texture(ce_ShadowMap, tc) * sampleStrength;
					strength += sampleStrength;
				}
			}
		}

		color /= strength;
		color.a = 1.0;

		ce_FragColor = color;
	}
}