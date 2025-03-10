#version 330

layout(location = eVS_Vertices) in vec3 cs_Position;
layout(location = eVS_UV) in vec3 cs_TexCoord;

uniform vec2 cs_Scale;
uniform vec2 cs_Translation;

out vec3 texCoord;

void main ()
{
	gl_Position = vec4(cs_Position * vec3(cs_Scale, 1.0) + vec3(cs_Translation, 0.0), 1.0);
	texCoord = cs_TexCoord;
}

