#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;

uniform mat4 spc_ModelMatrix;


void main()
{
	gl_Position = spc_ModelMatrix * spc_Position;
}

