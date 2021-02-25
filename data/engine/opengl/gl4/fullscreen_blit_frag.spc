shader "fragment" "
#version 330
layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;


in vec2 texCoord;

void main ()
{
	spc_FragColor = texture(spc_Diffuse, texCoord);
}

"