shader "vertex" "
#version 330

layout(location = eVS_Vertices) in vec3 spc_Position;
layout(location = eVS_UV) in vec3 spc_TexCoord;


out vec3 texCoord;

void main ()
{
	gl_Position = vec4(spc_Position, 1.0);
	texCoord = spc_TexCoord;
}

"