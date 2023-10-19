#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DiffuseRoughness;
uniform sampler2D ce_Normal;
uniform sampler2D ce_Depth;

uniform mat4 ce_ViewProjectionMatrixInv;

uniform vec3 ce_NegLightDirection;

in vec2 texCoord;

in vec2 ndc;


void main ()
{
    //
    // decompose the world position
    float z = texture(ce_Depth, texCoord).r;
    if (z == 1.0)
    {
        ce_FragColor = vec4(1.0, 0.0, 1.0, 1.0);

        return;
    }
    vec4 coord = vec4(ndc, z, 1.0);

    coord = ce_ViewProjectionMatrixInv * coord;
    coord /= coord.w;
    // coord now holds the world coordinate


    //
    // decompose the world normal
    vec3 normal = texture(ce_Normal, texCoord).xyz;
    normal = normal * 2.0 - 1.0;
    // normal now holds the world normal


    float lambert = clamp (dot (ce_NegLightDirection, normal), 0.0, 1.0);

    vec4 diffuseRoughness = texture(ce_DiffuseRoughness, texCoord);

    vec3 color = diffuseRoughness.xyz;


    ce_FragColor = vec4(color * lambert, 1.0);

}
