#version 330

layout(location = 0) out vec4 cs_FragDiffuseRoughness;
layout(location = 1) out vec4 cs_FragNormal;
layout(location = 2) out vec4 cs_FragEmission;

uniform sampler2D cs_Diffuse;
uniform sampler2D cs_Normal;
uniform sampler2D cs_RoughnessMap;
uniform vec4 cs_Color;
uniform float cs_Roughness;
uniform float cs_Metallic;


in vec2 texCoord;
in vec3 world_normal;
in vec3 world_tangent;

void main()
{
    //
    // Generate diffuse roughness
    float roughness = texture(cs_RoughnessMap, texCoord).r * cs_Roughness;
    vec4 color = texture(cs_Diffuse, texCoord) + cs_Color;
//    color = vec4(1.0, 0.8, 0.8, 1.0f);
    cs_FragDiffuseRoughness = vec4(color.rgb, roughness);

    //
    // Generate normals
    vec3 norm = normalize(world_normal);
    vec3 tang = normalize(world_tangent);
    vec3 binormal = normalize(cross(norm, tang));
    tang = cross(binormal, norm);

    mat3 normalMatrix = mat3(tang, binormal, norm);
    vec3 normal = texture(cs_Normal, texCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalMatrix * normal;
    cs_FragNormal = vec4(normal * 0.5 + 0.5, 1.0);


    //
    // Generate emission
    cs_FragEmission = vec4(0, 0, 0, 0);


}



