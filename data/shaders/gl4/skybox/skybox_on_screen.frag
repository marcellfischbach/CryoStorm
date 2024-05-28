#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform samplerCube ce_Skybox;

in vec3 uv;

void main ()
{
    ce_FragColor = texture(ce_Skybox, uv);
//    ce_FragColor += vec4(uv * 0.5 + 0.5, 1.0);
}