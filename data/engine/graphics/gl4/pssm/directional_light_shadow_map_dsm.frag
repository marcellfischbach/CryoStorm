#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform vec4 cs_LayersDepth;
uniform float cs_LayersBias;
uniform mat4 cs_MappingMatrices[4];
uniform mat4 cs_ShadowMapViewProjectionMatrix[4];
uniform sampler2DArrayShadow cs_ShadowBuffers;
uniform sampler2DArray cs_ShadowBufferDatas;
uniform sampler2D cs_DepthBuffer;
uniform vec2 cs_ShadowDepthRange;
uniform vec2 cs_ShadowBufferPixelSize;
uniform vec4 cs_ShadowBufferDepthBias;
uniform vec2 cs_BlurFactor;


uniform mat4 cs_ViewMatrix;
uniform mat4 cs_ViewProjectionMatrixInv;


in vec2 texCoord;



float calc_directional_shadow(vec3 world_position, float distance_to_camera, vec2 displacement, float addBias)
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
    camSpace.xy += displacement;

    float bias = texture(cs_ShadowBufferDatas, vec3(camSpace.xy, layer)).x; 
    camSpace.z -= (bias + 0.0001 + addBias);
    float shadow_value = texture(cs_ShadowBuffers, vec4(camSpace.xy, layer , camSpace.z));
    return mix(shadow_value, 1.0, fadeOut);
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

    float factor = 0.0;
    float shadow = 0.0;

    const vec2 offset[] = vec2[](
        vec2(0.0, 0.0),
        vec2(-1.0, -1.0),
        vec2(1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0)
    );
    const float[] factors = float[](
        1.0,
        0.25,
        0.25,
        0.25,
        0.25
    );

    // cs_ShadowDepthRange 
    //  x: minimum distance
    //  y: difference between min and max distance

    float depthFactor = clamp((camera_space.z - cs_ShadowDepthRange.x) / cs_ShadowDepthRange.y, 0.0, 1.0);
    
    float bias = cs_ShadowBufferDepthBias.x + cs_ShadowBufferDepthBias.y * depthFactor;
    
    float blurFactor = (cs_BlurFactor.x + cs_BlurFactor.y * depthFactor);
    vec2 blur = blurFactor * cs_ShadowBufferPixelSize;

    for (int i = 0; i < 5; i++)
    {
        float f = factors[i];
        vec2 displacement = offset[i]  * blur;
        shadow += calc_directional_shadow(world_position.xyz, camera_space.z, displacement, bias) * f;
        factor += f;

    }

    shadow /= factor;
    cs_FragColor = vec4(shadow, shadow, shadow, 1.0);
}

