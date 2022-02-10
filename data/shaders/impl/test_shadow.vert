#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;

uniform mat4 ce_ModelViewProjectionMatrix;


void main()
{
	gl_Position = ce_ModelViewProjectionMatrix * ce_Position;
}

