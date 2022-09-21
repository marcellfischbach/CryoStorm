#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform vec4 ce_LayersBias;
uniform mat4 ce_MappingMatrices[3];
uniform mat4 ce_ShadowMapViewProjectionMatrix[3];
uniform sampler2DArrayShadow ce_ShadowBuffer;
uniform sampler2D ce_DepthBuffer;


uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ViewProjectionMatrixInv;


in vec2 texCoord;

bool is_valid (vec3 v)
{
    return v.x >= 0.0 && v.x <= 1.0
     && v.y >= 0.0 && v.y <= 1.0;
}

vec3 to_cam_space (vec3 world_position, int idx)
{
    vec4 camSpace = ce_ShadowMapViewProjectionMatrix[idx] * vec4(world_position, 1.0);
    camSpace /= camSpace.w;
    camSpace = camSpace * 0.5 + 0.5;
    camSpace.z -= ce_LayersBias.w;
    return camSpace.xyz;
}

vec3 calc_directional_shadow(vec3 world_position, float distance_to_camera)
{
    vec4 layerBias = ce_LayersBias;

    float fadeOut = 0.0f;
    float layer = 0.0;
    int matIndex = 0;

    if (distance_to_camera <= layerBias.x)
    {
        matIndex = 0;
        layer = 0.0;
    }
    else if (distance_to_camera <= layerBias.y)
    {
        matIndex = 1;
        layer = 1.0;
    }
    else if (distance_to_camera <= layerBias.z)
    {
        matIndex = 2;
        layer = 2.0;

        fadeOut = smoothstep(layerBias.z - (layerBias.z - layerBias.y) * 0.1, layerBias.z, distance_to_camera);
    }
    else
    {
        return vec3(1.0);
    }

    vec4 camSpace = ce_ShadowMapViewProjectionMatrix[matIndex] * vec4(world_position, 1.0);
    camSpace /= camSpace.w;
    camSpace = camSpace * 0.5 + 0.5;
    camSpace.z -= layerBias.w;

    float shadow_value = texture(ce_ShadowBuffer, vec4(camSpace.xy, layer, camSpace.z));
    return vec3(mix(shadow_value, 1.0, fadeOut));
}


void main ()
{
    float depth = texture(ce_DepthBuffer, texCoord).r;
    if (depth == 1.0)
    {
        ce_FragColor = vec4(1, 1, 1, 1);
        return;
    }

    vec4 proj_space = vec4(texCoord.xy, depth, 1.0);
    proj_space = proj_space * 2.0 - 1.0;
    vec4 world_position = ce_ViewProjectionMatrixInv * proj_space;
    world_position /= world_position.w;

    vec4 camera_space = ce_ViewMatrix * world_position;

    vec3 shadow = calc_directional_shadow(world_position.xyz, camera_space.z);
    ce_FragColor = vec4(shadow, 1.0);
}

