#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform samplerCube cs_Skybox;

in vec3 uv;

void main ()
{
    cs_FragColor = texture(cs_Skybox, uv);
}