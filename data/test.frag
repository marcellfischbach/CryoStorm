#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;

in vec4 color;
in vec2 texCoord;

void main()
{
  spc_FragColor = texture(spc_Diffuse, texCoord) * spc_Color;
  //spc_FragColor = spc_Color;
}

