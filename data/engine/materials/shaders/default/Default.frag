#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Diffuse;
uniform sampler2D cs_Normal;
uniform sampler2D cs_RoughnessMap;
uniform vec4 cs_Color;
uniform float cs_Roughness;
uniform float cs_Metallic;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 world_tangent;
in vec3 camera_space_position;
in vec3 viewer_world_position;
in vec2 screen_coord;

#include </shaders/common/lighting.glsl>

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_l, float h_dot_v)
{
    float roughness = texture(cs_RoughnessMap, texCoord).r;
    roughness = roughness * cs_Roughness;

    lighting_result_t res;
    res.ambient = 0.0;
    res.diffuse = oren_nayar (n_dot_l, n_dot_v, roughness);
    res.specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);
    return res;
}


void main()
{
    vec3 norm = normalize(world_normal);
    vec3 tang = normalize(world_tangent);
    vec3 binormal = normalize(cross(norm, tang));
    tang = cross(binormal, norm);

    mat3 normalMatrix = mat3(tang, binormal, norm);
    vec3 normal = texture(cs_Normal, texCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalMatrix * normal;



    light_result_t light = calc_lights(world_position, normal, camera_space_position, viewer_world_position);
    vec4 color = texture(cs_Diffuse, texCoord) + cs_Color;


    vec3 dielectric = light.diffuse  * color.rgb + light.specular;
    vec3 metallic = light.specular * color.rgb;


    cs_FragColor = vec4(mix(dielectric, metallic, cs_Metallic), color.a);
}





