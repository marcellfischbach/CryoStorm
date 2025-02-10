#version 330

layout(location = 0) out vec4 cs_FragDiffuseRoughness;
layout(location = 1) out vec4 cs_FragNormal;
layout(location = 2) out vec4 cs_FragEmission;


uniform sampler2D cs_Layer;
uniform sampler2D cs_Mask;
uniform sampler2D cs_DiffuseRoughness0;
uniform sampler2D cs_DiffuseRoughness1;
uniform sampler2D cs_DiffuseRoughness2;
uniform sampler2D cs_DiffuseRoughness3;
uniform sampler2D cs_Normal0;
uniform sampler2D cs_Normal1;
uniform sampler2D cs_Normal2;
uniform sampler2D cs_Normal3;


in vec2 texCoord;
in vec3 world_normal;

void main()
{
    vec3 norm = normalize(world_normal);
    cs_FragNormal = vec4(norm * 0.5 + 0.5, 0.0);


    vec4 layer = texture(cs_Layer, texCoord);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    float loc_roughness = 0.0;
    if (layer.x > 0.0)
    {
        vec4 diffuseRoughness = texture (cs_DiffuseRoughness0, texCoord * 30);
        diffuse += diffuseRoughness.rgb * layer.x;
        loc_roughness += diffuseRoughness.a * layer.x;
    }
    if (layer.y > 0.0)
    {
        vec4 diffuseRoughness = texture (cs_DiffuseRoughness1, texCoord * 30);
        diffuse += diffuseRoughness.rgb * layer.y;
        loc_roughness += diffuseRoughness.a * layer.y;
    }
    if (layer.z > 0.0)
    {
        vec4 diffuseRoughness = texture (cs_DiffuseRoughness2, texCoord * 30);
        diffuse += diffuseRoughness.rgb * layer.z;
        loc_roughness += diffuseRoughness.a * layer.z;
    }
//    if (layer.w > 0.0)
//    {
//        vec4 diffuseRoughness = texture (cs_DiffuseRoughness3, texCoord * 3);
//        diffuse += diffuseRoughness.rgb * layer.w;
//        roughness += diffuseRoughness.a * layer.w;
//    }
//    diffuse = vec3(1, 1, 1);

    cs_FragDiffuseRoughness = vec4(diffuse, loc_roughness);
//    cs_FragDiffuseRoughness = vec4(loc_roughness, loc_roughness, loc_roughness, 1.0);
    cs_FragEmission = vec4(0.0, 0.0, 0.0, 0.0);
}

