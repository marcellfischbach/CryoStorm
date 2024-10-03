#version 330

layout (location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_DepthBuffer;
uniform sampler2D cs_ShadowMap;
uniform mat4 cs_ProjectionMatrixInv;
uniform float cs_FilterRadius;
uniform float cs_FilterSamples;
uniform vec2 cs_FilterDistance;
uniform float cs_FilterMaxSampleDistance;
uniform float cs_ScreenAspect;
uniform float cs_Random;

in vec2 texCoord;



vec3 get_position_in_camera_space(vec2 uv)
{
    float depth = texture(cs_DepthBuffer, uv).r;
    if (depth == 1.0)
    {
        return vec3(0, 0, -1);
    }
    vec4 proj_space = vec4(uv.xy, depth, 1.0);
    proj_space = proj_space * 2.0 - 1.0;
    vec4 cam_space = cs_ProjectionMatrixInv * proj_space;
    return cam_space.xyz / cam_space.w;

}

void main ()
{
	vec3 position_in_camera_space = get_position_in_camera_space(texCoord);
	if (position_in_camera_space == vec3(0, 0, -1))
	{
	  cs_FragColor = vec4(1, 1, 1, 1);
		return;
	}
	float distance_to_camera = position_in_camera_space.z;


	// get num and size based on distance_to_camera
	float frac = clamp((distance_to_camera - cs_FilterDistance.x) / cs_FilterDistance.y, 0.0, 1.0);
	float num = mix(cs_FilterSamples, 1, frac);
	float size = mix(cs_FilterRadius, 0.0, frac);

	float radius_x = size;
	float radius_y = size * cs_ScreenAspect;

	float rnd = (texCoord.x + texCoord.y) * cs_Random * 0.5;

	vec4 color = vec4(0, 0, 0, 0);
	float total_strength = 0.0;
	for (float i=0; i<num; i++)
	{
		float r = i * 3.14 * 234.45;
		float s = i / num;

		vec2 sampleCoordinate = texCoord + vec2(cos(r) * s * radius_x, sin(r) * s * radius_y);

		vec3 sample_position_in_camera_space = get_position_in_camera_space(sampleCoordinate);

		float dist = length (sample_position_in_camera_space - position_in_camera_space);

		float sample_strength = clamp(1.0 - (dist / cs_FilterMaxSampleDistance), 0.0, 1.0);

		color += texture(cs_ShadowMap, sampleCoordinate) * sample_strength;
		total_strength += sample_strength;
	}

	if (total_strength != 0.0)
	{
	  cs_FragColor = color / total_strength;
	}

}