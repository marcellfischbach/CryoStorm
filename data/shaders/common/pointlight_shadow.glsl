
uniform vec3 spc_PointLightShadowMapMappingBias[4];
uniform samplerCubeShadow spc_PointLightShadowMapDepth[4];


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

        vec3 mapping = spc_PointLightShadowMapMappingBias[idx];
        float n = mapping.x;
        float f = mapping.y;

        float z = get_major(delta);
        float fz = (z * (f+n) - 2.0*n*f)/(f-n);
        float fw = z;
        fz = fz / fw;
        fz = fz * 0.5 + 0.5;
        fz -= mapping.z;


        float v = texture(spc_PointLightShadowMapDepth[idx], vec4(delta, fz));
        return v;
    }
}