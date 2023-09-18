#version 330

layout(location = 0) out vec4 ce_FragDiffuseRoughness;
layout(location = 1) out vec4 ce_FragNormal;
layout(location = 2) out vec4 ce_FragEmission;

uniform sampler2D ce_Diffuse;
uniform sampler2D ce_Normal;
uniform sampler2D ce_RoughnessMap;
uniform vec4 ce_Color;
uniform float ce_Roughness;
uniform float ce_Metallic;


in vec2 texCoord;
in vec3 world_normal;
in vec3 world_tangent;

void main()
{
    //
    // Generate diffuse roughness
    float roughness = texture(ce_RoughnessMap, texCoord * 3).r;
    roughness = roughness * ce_Roughness;

    vec4 color = texture(ce_Diffuse, texCoord * 3) * ce_Color;
    ce_FragDiffuseRoughness = vec4(color.rgb, roughness);

    //
    // Generate normals
    vec3 norm = normalize(world_normal);
    vec3 tang = normalize(world_tangent);
    vec3 binormal = normalize(cross(norm, tang));
    tang = cross(binormal, norm);

    mat3 normalMatrix = mat3(tang, binormal, norm);
    vec3 normal = texture(ce_Normal, texCoord*3).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalMatrix * normal;
    ce_FragNormal = vec4(normal * 0.5 + 0.5, 1.0);


    //
    // Generate emission
    ce_FragEmission = vec4(0, 0, 0, 0);



}



