#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DepthBuffer;
uniform sampler2D ce_ShadowMap;
uniform mat4 ce_ProjectionMatrixInv;
uniform float ce_FilterRadius;
uniform float ce_FilterSamples;
uniform vec2 ce_FilterDistance;
uniform float ce_FilterMaxSampleDistance;
uniform float ce_ScreenAspect;
uniform float ce_Random;

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


	// get num and size based on distance_to_camera
	float frac = clamp((distance_to_camera - ce_FilterDistance.x) / ce_FilterDistance.y, 0.0, 1.0);
	float num = mix(ce_FilterSamples, 1, frac);
	float size = mix(ce_FilterRadius, 0.0, frac);

	float radius_x = size;
	float radius_y = size * ce_ScreenAspect;

	float rnd = (texCoord.x + texCoord.y) * ce_Random * 0.5;

	vec4 color = vec4(0, 0, 0, 0);
	float total_strength = 0.0;
	for (float i=0; i<num; i++)
	{
		float r = i * 3.14 * 234.45;
		float s = i / num;

		vec2 sampleCoordinate = texCoord + vec2(cos(r) * s * radius_x, sin(r) * s * radius_y);

		vec3 sample_position_in_camera_space = get_position_in_camera_space(sampleCoordinate);

		float dist = length (sample_position_in_camera_space - position_in_camera_space);

		float sample_strength = clamp(1.0 - (dist / ce_FilterMaxSampleDistance), 0.0, 1.0);

		color += texture(ce_ShadowMap, sampleCoordinate) * sample_strength;
		total_strength += sample_strength;
	}

	if (total_strength != 0.0)
	{
		ce_FragColor = color / total_strength;
	}

}