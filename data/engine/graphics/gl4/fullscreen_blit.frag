#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Diffuse;


in vec2 texCoord;

void main ()
{
  cs_FragColor = texture(cs_Diffuse, texCoord);
}

