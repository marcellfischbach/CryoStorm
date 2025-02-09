#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform vec4 cs_LayersDepth;
uniform float cs_LayersBias;
uniform mat4 cs_MappingMatrices[4];
uniform mat4 cs_ShadowMapViewProjectionMatrix[4];
uniform sampler2DArrayShadow cs_ShadowBuffers;
uniform sampler2DArray cs_ShadowBufferDatas;
uniform sampler2D cs_DepthBuffer;


uniform mat4 cs_ViewMatrix;
uniform mat4 cs_ViewProjectionMatrixInv;

const float g_MinVariance = 0.0;

in vec2 texCoord;

float ChebyshevUpperBound(vec2 Moments, float distance_to_light)
{
    // One-tailed inequality valid if t > Moments.x
    float p = (distance_to_light <= Moments.x) ? 1.0 : 0.0;
    // Compute variance.
    float Variance = Moments.y - (Moments.x * Moments.x);
    Variance = max(Variance, g_MinVariance);
    // Compute probabilistic upper bound.
    float d = distance_to_light - Moments.x;
    float p_max = Variance / (Variance + d * d);
    return p;
    //return max(p, p_max);
}

float calc_directional_shadow(vec3 world_position, float distance_to_camera)
{
    vec4 layerDepth = cs_LayersDepth;

    float fadeOut = 0.0f;
    int layer = 0;
    int matIndex = 0;

    if (distance_to_camera <= layerDepth.x)
    {
        matIndex = 0;
        layer = 0;
    }
    else if (distance_to_camera <= layerDepth.y)
    {
        matIndex = 1;
        layer = 1;
    }
    else if (distance_to_camera <= layerDepth.z)
    {
        matIndex = 2;
        layer = 2;
    }
    else if (distance_to_camera <= layerDepth.w)
    {
        matIndex = 3;
        layer = 3;

        fadeOut = smoothstep(layerDepth.w - (layerDepth.w - layerDepth.z) * 0.1, layerDepth.w, distance_to_camera);
    }
    else
    {
        return 1.0;
    }

    vec4 camSpace = cs_ShadowMapViewProjectionMatrix[matIndex] * vec4(world_position, 1.0);
    camSpace /= camSpace.w;
    camSpace  = camSpace * 0.5 + 0.5;
//    camSpace.z -= cs_LayersBias;

    vec2 moments = texture (cs_ShadowBufferDatas, vec3(camSpace.xy, layer)).xy;
    return moments.x * 1000000.0;
    return ChebyshevUpperBound(moments, camSpace.z);

//    float shadow_value = texture(cs_ShadowBuffers, vec4(camSpace.xy, layer , camSpace.z));
//    return mix(shadow_value, 1.0, fadeOut);
}


void main ()
{
    float depth = texture(cs_DepthBuffer, texCoord).r;
    if (depth == 1.0)
    {
        cs_FragColor = vec4(1, 1, 1, 1);
        return;
    }

    vec4 proj_space = vec4(texCoord.xy, depth, 1.0);
    proj_space = proj_space * 2.0 - 1.0;
    vec4 world_position = cs_ViewProjectionMatrixInv * proj_space;
    world_position /= world_position.w;

    vec4 camera_space = cs_ViewMatrix * world_position;

    float shadow = calc_directional_shadow(world_position.xyz, camera_space.z);
    cs_FragColor = vec4(shadow, shadow, shadow, 1.0);
}

