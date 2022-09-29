#version 330
layout(location = 0) out vec4 ce_FragColor;


uniform vec3 ce_LightPosition;
uniform vec3 ce_MappingBias;
uniform samplerCubeShadow ce_ShadowBuffer;
uniform sampler2D ce_DepthBuffer;


uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ViewProjectionMatrixInv;


in vec2 texCoord;




float get_major(vec3 d)
{
    vec3 ad = abs(d);
    if (ad.x > ad.y && ad.x > ad.z)
    {
        return ad.x;
    }
    else if (ad.y > ad.x && ad.y > ad.z)
    {
        return ad.y;
    }
    return ad.z;
}


float calc_point_shadow(vec3 frag_position)
{
    vec3 delta = frag_position - light_position;
    delta.z = -delta.z;

    float n = ce_MappingBias.x;
    float f = ce_MappingBias.y;

    float z = get_major(delta);
    float fz = (z * (f+n) - 2.0*n*f)/(f-n);
    float fw = z;
    fz = fz / fw;
    fz = fz * 0.5 + 0.5;
    fz -= ce_MappingBias.z;


    return texture(ce_ShadowBuffer, vec4(delta, fz));
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


    float shadow = calc_point_shadow(world_position.xyz);
    ce_FragColor = vec4(shadow, shadow, shadow, 1.0);
}

