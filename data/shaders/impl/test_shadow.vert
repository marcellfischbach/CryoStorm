#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;

uniform mat4 spc_ModelViewProjectionMatrix;


void main()
{
	gl_Position = spc_ModelViewProjectionMatrix * spc_Position;
}

