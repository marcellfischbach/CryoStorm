#version 330

layout(location = 0) out vec4 ce_FragDiffuseRoughness;
layout(location = 1) out vec4 ce_FragNormal;
layout(location = 2) out vec4 ce_FragEmission;


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


in vec2 texCoord;
in vec3 world_normal;

void main()
{
    vec3 norm = normalize(world_normal);
    ce_FragNormal = vec4(norm * 0.5 + 0.5, 0.0);


    vec4 layer = texture(ce_Layer, texCoord);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    float loc_roughness = 0.0;
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


    ce_FragDiffuseRoughness = vec4(diffuse, loc_roughness);
//    ce_FragDiffuseRoughness = vec4(loc_roughness, loc_roughness, loc_roughness, 1.0);
    ce_FragEmission = vec4(0.0, 0.0, 0.0, 0.0);
}

