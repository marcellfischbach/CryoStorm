#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DepthBuffer;
uniform sampler2D ce_ShadowMap;
uniform mat4 ce_ProjectionMatrixInv;
uniform float ce_FilterRadius;
uniform float ce_FilterSamples;
uniform vec2 ce_FilterDistance;
uniform float ce_ScreenAspect;
uniform float ce_Random;

in vec2 texCoord;


void main ()
{

	//
	// get the distance_to_camera
    float depth = texture(ce_DepthBuffer, texCoord).r;
    if (depth == 1.0)
    {
        ce_FragColor = vec4(1, 1, 1, 1);
        return;
    }
    vec4 proj_space = vec4(texCoord.xy, depth, 1.0);
    proj_space = proj_space * 2.0 - 1.0;
    vec4 cam_space = ce_ProjectionMatrixInv * proj_space;
    float distance_to_camera = cam_space.z /= cam_space.w;


	// get num and size based on distance_to_camera
	float frac = clamp((distance_to_camera - ce_FilterDistance.x) / ce_FilterDistance.y, 0.0, 1.0);
	float num = mix(ce_FilterSamples, 1, frac);
	float size = mix(ce_FilterRadius, 0.0, frac);

//	float frac = clamp(distance_to_camera - 1.0 / 24.0, 0.0, 1.0);
//	int num = int(clamp(mix(20, 1, frac), 1, 20));
//	float size = mix(25.0, 1.0, frac);



	

	float radius_x = size;
	float radius_y = size * ce_ScreenAspect;

	float rnd = (texCoord.x + texCoord.y) * ce_Random * 0.5;

	vec4 color = vec4(0, 0, 0, 0);
	for (float i=0; i<num; i++)
	{
		float r = rnd + i * 3.14 * 234.45;
		float s = i / num;

		vec2 add = vec2(cos(r) * s * radius_x, sin(r) * s * radius_y);
		color += texture(ce_ShadowMap, texCoord + add);
	}

	ce_FragColor = color / num;


}