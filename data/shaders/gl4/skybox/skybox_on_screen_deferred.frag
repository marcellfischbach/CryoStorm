#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform samplerCube cs_Skybox;
uniform sampler2D cs_Depth;

in vec3 uv;
in vec4 fragCoord;

void main ()
{
    vec2 fc = (fragCoord.xy / fragCoord.w) * 0.5 + 0.5;
    float d = texture(cs_Depth, fc).r;
    if (d != 1.0)
    {
        cs_FragColor = vec4(0, 0, 0, 0);
    }
    else
    {
        cs_FragColor = texture(cs_Skybox, uv);
    }
}