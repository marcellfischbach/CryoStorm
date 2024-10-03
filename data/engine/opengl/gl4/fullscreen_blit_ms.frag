#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2DMS  cs_Diffuse;
uniform int cs_Samples;


in vec2 texCoord;

void main ()
{
	ivec2 size = textureSize (cs_Diffuse);
	vec4 color = vec4(0, 0, 0, 0);

	for (int i=0; i<ce_Samples; i++)
	{
	    color += texelFetch(cs_Diffuse, ivec2(texCoord * size), i);
	}
  cs_FragColor = color / cs_Samples;
}

