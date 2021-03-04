shader "fragment" "
#version 330
layout(location = 0) out vec4 spc_FragColor;

uniform sampler2DArray spc_Diffuse;
uniform float spc_ArrayIndex;


in vec2 texCoord;

void main ()
{
	spc_FragColor = texture(spc_Diffuse, vec3(texCoord, 1.0));
}

"