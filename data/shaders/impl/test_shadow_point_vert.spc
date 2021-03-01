
shader "vertex" "
#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;
layout(location = eVS_UV) in vec2 spc_UV;

uniform mat4 spc_ModelMatrix;

out vec2 geomUV;

void main()
{
	gl_Position = spc_ModelMatrix * spc_Position;
	geomUV = spc_UV;
}





"