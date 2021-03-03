fragment "



uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCastShadow[4];

uniform samplerCubeShadow spc_PointLightShadowMapDepth[4];

uniform int spc_LightCount;




vec4 calc_point_shadow(int idx, vec3 light_position, float light_range, vec3 frag_position)
{
    if (spc_LightCastShadow[idx] == 0)
    {
        return vec4(0, 1, 0, 1);
    }
    else
    {
        vec3 delta = frag_position - light_position;
        float distance = 1.0 - (length(delta) - 1.0) / (light_range - 1.0) * 2.0f - 1.0f;
        delta *= vec3(1, -1, 1);
        delta = normalize(delta);


        float shd = texture(spc_PointLightShadowMapDepth[idx], vec4(delta.xy, delta.z, distance));
        return vec4(shd);
    }
}


vec4 calc_point_light(vec4 light_color, vec3 light_position, float light_range, vec3 frag_position, vec3  frag_normal)
{
    vec3 v_to_l = light_position - frag_position;
    float distance = length(v_to_l);

    v_to_l /= distance;
    return light_color
            * clamp(dot(v_to_l, frag_normal), 0.0, 1.0)
            * max(1.0 - distance / light_range, 0.0);
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

"