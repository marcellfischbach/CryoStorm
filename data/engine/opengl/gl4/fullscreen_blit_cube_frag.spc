shader "fragment" "
#version 330
layout(location = 0) out vec4 spc_FragColor;

uniform samplerCube spc_Diffuse;


in vec3 texCoord;

void main ()
{
	vec3 tex = normalize(texCoord);
	spc_FragColor = texture(spc_Diffuse, texCoord);
}

"