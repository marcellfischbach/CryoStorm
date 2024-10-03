#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;

uniform mat4 cs_ModelViewProjectionMatrix;


void main()
{
	gl_Position = cs_ModelViewProjectionMatrix * cs_Position;
}

