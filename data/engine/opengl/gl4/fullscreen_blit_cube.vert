#version 330

layout(location = eVS_Vertices) in vec3 ce_Position;
layout(location = eVS_UV) in vec3 ce_TexCoord;


out vec3 texCoord;

void main ()
{
	gl_Position = vec4(ce_Position, 1.0);
	texCoord = ce_TexCoord;
}
