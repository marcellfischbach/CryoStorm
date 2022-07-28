#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Layer;
uniform sampler2D ce_Mask;
uniform sampler2D ce_DiffuseRoughness0;
uniform sampler2D ce_DiffuseRoughness1;
uniform sampler2D ce_DiffuseRoughness2;
uniform sampler2D ce_DiffuseRoughness3;
uniform sampler2D ce_Normal0;
uniform sampler2D ce_Normal1;
uniform sampler2D ce_Normal2;
uniform sampler2D ce_Normal3;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 camera_space_position;
in vec3 viewer_world_position;

#include <../common/lighting.glsl>

float loc_roughness = 0.0;

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_l, float h_dot_v)
{
    lighting_result_t res;
    res.ambient = 0.0;
    res.diffuse = oren_nayar (n_dot_l, n_dot_v, loc_roughness);
    res.specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_v, loc_roughness);
    return res;
}


void main()
{
    vec3 norm = normalize(world_normal);

    vec4 layer = texture(ce_Layer, texCoord);

    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    loc_roughness = 0.0;
    if (layer.x > 0.0)
    {
        vec4 diffuseRoughness = texture (ce_DiffuseRoughness0, texCoord * 3);
        diffuse += diffuseRoughness.rgb * layer.x;
        loc_roughness += diffuseRoughness.a * layer.x;
    }
    if (layer.y > 0.0)
    {
        vec4 diffuseRoughness = texture (ce_DiffuseRoughness1, texCoord * 3);
        diffuse += diffuseRoughness.rgb * layer.y;
        loc_roughness += diffuseRoughness.a * layer.y;
    }
    if (layer.z > 0.0)
    {
        vec4 diffuseRoughness = texture (ce_DiffuseRoughness2, texCoord * 3);
        diffuse += diffuseRoughness.rgb * layer.z;
        loc_roughness += diffuseRoughness.a * layer.z;
    }
//    if (layer.w > 0.0)
//    {
//        vec4 diffuseRoughness = texture (ce_DiffuseRoughness3, texCoord * 3);
//        diffuse += diffuseRoughness.rgb * layer.w;
//        roughness += diffuseRoughness.a * layer.w;
//    }


    light_result_t light = calc_lights(world_position, norm, camera_space_position, viewer_world_position);
    vec3 dielectric = light.diffuse  * diffuse.rgb + light.specular;
    ce_FragColor = vec4(dielectric, color.a);


//    ce_FragColor =  vec4(frag_light * diffuse, 1.0);
    //    ce_FragColor = vec4(frag_light, 1.0) * ce_Color;// * texColor;
    //    ce_FragColor = vec4(norm * 0.5 + 0.5, 1.0) * texColor;
}

