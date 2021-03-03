#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;

in vec2 fragUV;


void main()
{
	spc_FragColor = vec4(1, 1, 1, 1);
	spc_FragColor = texture(spc_Diffuse, fragUV);
	spc_FragColor = vec4(fragUV * 0.5 + 0.5, 0, 1);
}

