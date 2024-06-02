#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform samplerCube ce_Skybox;
uniform sampler2D ce_Depth;

in vec3 uv;
in vec4 fragCoord;

void main ()
{
    vec2 fc = (fragCoord.xy / fragCoord.w) * 0.5 + 0.5;
    float d = texture(ce_Depth, fc).r;
    if (d != 1.0)
    {
        ce_FragColor = vec4(0, 0, 0, 0);
    }
    else
    {
        ce_FragColor = texture(ce_Skybox, uv);
    }
}