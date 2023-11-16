#version 330

layout(location = eVS_Vertices) in vec3 ce_Position;
layout(location = eVS_UV) in vec3 ce_TexCoord;

uniform vec2 ce_Scale;
uniform vec2 ce_Translation;

out vec3 texCoord;

void main ()
{
	gl_Position = vec4(ce_Position * vec3(ce_Scale, 1.0) + vec3(ce_Translation, 0.0), 1.0);
	texCoord = ce_TexCoord;
}

