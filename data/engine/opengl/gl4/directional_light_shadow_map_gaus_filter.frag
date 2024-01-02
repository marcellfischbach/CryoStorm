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

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main ()
{

	int size = int(ce_FilterSamples);

	if (size == 0)
	{
		ce_FragColor = texture(ce_ShadowMap, texCoord);
	}
	else
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

		vec4 color;
		float strength = 0.0;
		for (int i=-size; i<=size; i++)
		{
			float fi = float(i) / float(size);
			float x = fi;
			float sampleX = x * filter_radius;
			for (int j=-size; j<=size; j++)
			{
				float fj = float(j) / float(size);
				float y = fj;
				float sampleY = y * filter_radius * ce_ScreenAspect;

				float sampleStrength = max(1.0 - (x*x + y*y), 0.0);

				if (sampleStrength != 0.0)
				{
					color += texture(ce_ShadowMap, texCoord + vec2 (sampleX, sampleY)) * sampleStrength;
					strength += sampleStrength;
				}
			}
		}

		color /= strength;
		color.a = 1.0;

		ce_FragColor = color;
	}
}