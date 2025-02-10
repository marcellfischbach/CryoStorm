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

const float g_VSMMinVariance = 0.000001;// Minimum variance for VSM

const float g_LBRAmount = 0.5;

in vec2 texCoord;

float linstep(float min, float max, float v)
{
    return clamp((v - min) / (max - min), 0, 1);
}

// Light bleeding reduction
float LBR(float p)
{
    // Lots of options here if we don't care about being an upper bound.
    // Use whatever falloff function works well for your scene.
    return linstep(g_LBRAmount, 1, p);
    //return smoothstep(g_LBRAmount, 1, p);
}


float ChebyshevUpperBound(vec2 shadow_buffer, float distance_to_light, float MinVariance)
{
    // Standard shadow map comparison
    float p = (distance_to_light <= shadow_buffer.x) ? 1.0 : 0.0;
//    return p;


    // Compute variance
    float Variance = shadow_buffer.y - (shadow_buffer.x * shadow_buffer.x);
    Variance = max(Variance, MinVariance);
//    Variance = MinVariance;

    // Compute probabilistic upper bound
    float d     = distance_to_light - shadow_buffer.x;
    float p_max = Variance / (Variance + d*d);

    return max(p, p_max);
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
    camSpace = camSpace * 0.5 + 0.5;


    vec2 moments = texture (cs_ShadowBufferDatas, vec3(camSpace.xy, layer)).xy + vec2(cs_LayersBias, 0.0);
    float shadow = ChebyshevUpperBound(moments, camSpace.z, g_VSMMinVariance);

//    shadow = LBR(shadow);
    return shadow;

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

