#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;

uniform mat4 ce_ModelMatrix;


void main()
{
	gl_Position = ce_ModelMatrix * ce_Position;
}

