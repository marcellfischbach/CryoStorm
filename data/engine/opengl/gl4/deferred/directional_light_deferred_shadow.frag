
#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DiffuseRoughness;
uniform sampler2D ce_Normal;
uniform sampler2D ce_Depth;
uniform sampler2D ce_ShadowMap;

uniform mat4 ce_ViewProjectionMatrixInv;

uniform vec3 ce_CameraPosition;
uniform vec4 ce_LightColor;
uniform vec3 ce_NegLightDirection;


in vec2 texCoord;

in vec2 ndc;

#include<../../../../shaders/gl4/common/cook-torrance.glsl>
#include<../../../../shaders/gl4/common/oren-nayar.glsl>




void main ()
{
    //
    // decompose the world position
    float z = texture(ce_Depth, texCoord).r;
    if (z == 1.0)
    {
        discard;
    }
    vec4 worldPosition = vec4(texCoord, z, 1.0);
    worldPosition = worldPosition * 2.0 - 1.0;

    worldPosition = ce_ViewProjectionMatrixInv * worldPosition;
    worldPosition /= worldPosition.w;
    // coord now holds the world coordinate


    //
    // decompose the world normal
    vec3 normal = texture(ce_Normal, texCoord).xyz;
    normal = normal * 2.0 - 1.0;
    // normal now holds the world normal

    vec4 diffuseRoughness = texture(ce_DiffuseRoughness, texCoord);

    vec3 to_viewer = normalize(ce_CameraPosition - worldPosition.xyz);


    vec3 H = normalize(ce_NegLightDirection.xyz + to_viewer);
    float n_dot_l = dot (normal, ce_NegLightDirection);
    float n_dot_v = dot (normal, to_viewer);
    float n_dot_h = dot(normal, H);
    float h_dot_l = dot(H, ce_NegLightDirection);
    float lambert = clamp (n_dot_l, 0.0, 1.0);
    float specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_l, diffuseRoughness.a);
    float diffuse = oren_nayar(n_dot_l, n_dot_v, diffuseRoughness.a);


    vec3 color = diffuseRoughness.xyz;
    float shadow = texture(ce_ShadowMap, texCoord).r;


    ce_FragColor = vec4(color * diffuse * ce_LightColor.rgb * shadow + specular, 1.0);

}
