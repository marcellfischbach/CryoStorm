#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2DArray cs_Diffuse;
uniform float cs_ArrayIndex;


in vec2 texCoord;

void main ()
{
  cs_FragColor = texture(cs_Diffuse, vec3(texCoord, 1.0));
}

