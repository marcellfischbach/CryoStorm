


uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCastShadow[4];

uniform samplerCube spc_PointLightShadowMapColor[4];
uniform samplerCubeShadow spc_PointLightShadowMapDepth[4];
uniform vec3 spc_PointLightShadowMapMapping[4];

uniform int spc_LightCount;


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


float calc_point_shadow(int idx, vec3 light_position, float light_range, vec3 frag_position)
{
    if (spc_LightCastShadow[idx] == 0)
    {
        return 1.0;
    }
    else
    {
        vec3 delta = frag_position - light_position;
        delta.z = -delta.z;

        vec3 mapping = spc_PointLightShadowMapMapping[idx];
        float n = mapping.x;
        float f = mapping.y;

        float z = get_major(delta);
        float fz = (z * (f+n) - 2.0*n*f)/(f-n);
        float fw = z;
        fz = fz / fw;
        fz = fz * 0.5 + 0.5;
        fz -= mapping.z;


        return texture(spc_PointLightShadowMapDepth[idx], vec4(delta, fz));
    }
}


vec4 calc_point_light(vec4 light_color, vec3 light_position, float light_range, vec3 frag_position, vec3  frag_normal)
{
    vec3 v_to_l = light_position - frag_position;
    float distance = length(v_to_l);

    v_to_l /= distance;
    return light_color
            * clamp(dot(v_to_l, frag_normal), 0.0, 1.0)
            * max(1.0 - distance / light_range, 0.0)
            ;
}


vec4 calc_directional_light(vec4 light_color, vec3 light_vector, vec3 frag_normal)
{
    return light_color * clamp(dot(light_vector, frag_normal), 0.0, 1.0);
}

vec4 calc_light(int idx, vec4 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal)
{
    if (light_vector.w == 1.0)
    {
        return calc_point_light(light_color, light_vector.xyz, light_range, frag_position, frag_normal)
         * calc_point_shadow(idx, light_vector.xyz, light_range, frag_position);
    }
    else
    {
        return calc_directional_light(light_color, light_vector.xyz, frag_normal);
    }
}

vec4 calc_lights(vec3 frag_position, vec3 frag_normal)
{
    vec4 res = vec4(0, 0, 0, 0);
    for (int i = 0; i < spc_LightCount; i++)
    {
        res += calc_light(i, spc_LightColor[i], spc_LightVector[i], spc_LightRange[i], frag_position, frag_normal);
    }
    return res;
}

