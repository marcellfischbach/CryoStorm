#version 330

layout (location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DiffuseRoughness;
uniform sampler2D ce_Normal;
uniform sampler2D ce_Depth;
uniform sampler2D ce_ShadowMap;

uniform mat4 ce_ViewProjectionMatrixInv;

uniform vec3 ce_CameraPosition;
uniform vec4 ce_LightColor;
uniform vec4 ce_LightAmbientColor;
uniform vec3 ce_LightPosition;
uniform float ce_LightRange;

in vec2 texCoord;

in vec2 ndc;


#include</${shaders}/common/cook-torrance.glsl>
#include</${shaders}/common/oren-nayar.glsl>

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

    vec3 to_light = ce_LightPosition - worldPosition.xyz;
    float distance_to_light = length(to_light);
    if (distance_to_light > ce_LightRange)
    {
        discard;
    }

    vec3 light_dir = normalize(to_light);

    vec3 H = normalize(light_dir + to_viewer);
    float n_dot_l = clamp(dot (normal, light_dir), 0, 1);
    float n_dot_v = clamp(dot (normal, to_viewer), 0, 1);
    float n_dot_h = clamp(dot(normal, H), 0, 1);
    float h_dot_l = clamp(dot(H, light_dir), 0, 1);
    float specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_l, diffuseRoughness.a);
    float diffuse = oren_nayar(n_dot_l, n_dot_v, diffuseRoughness.a);
    vec3 color = diffuseRoughness.rgb;
    float shadow = texture(ce_ShadowMap, texCoord).r;

    float range_factor = clamp(1.0 - (distance_to_light / ce_LightRange), 0.0, 1.0);

    ce_FragColor = vec4(
        range_factor * color * diffuse * shadow * ce_LightColor.rgb +
        range_factor * specular * shadow * ce_LightColor.rgb +
        ce_LightAmbientColor.rgb,
        1.0);

}

