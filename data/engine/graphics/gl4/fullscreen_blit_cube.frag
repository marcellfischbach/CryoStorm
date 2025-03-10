#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform samplerCube cs_Diffuse;


in vec3 texCoord;

void main ()
{
	vec3 tex = normalize(texCoord);
  cs_FragColor = texture(cs_Diffuse, texCoord);
}

