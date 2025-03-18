#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;

uniform mat4 cs_ModelMatrix;


void main()
{
	gl_Position = cs_ModelMatrix * cs_Position;
}

