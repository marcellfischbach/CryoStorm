
float oren_nayar (float n_dot_l, float n_dot_v, float roughness)
{
    float angleVN = acos(n_dot_v);
    float angleLN = acos(n_dot_l);

    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = cos(angleVN - angleLN);

    float roughness2 = roughness * roughness;
    float A = 1.0 - 0.5 * roughness2 / (roughness2 + 0.33);
    float B = 0.45 *      roughness2 / (roughness2 + 0.09);
    float C = sin(alpha) * tan(beta);
    return n_dot_l * (A + (B * max(0.0, gamma) * C));
}


/*

vec3 calc_lighting (vec3 light_color, vec3 frag_to_light, vec3 frag_normal, vec3 frag_to_viewer, float roughness)
{

    float NdotV = clamp(dot(frag_normal, frag_to_viewer), 0.0, 1.0);
    float NdotL = clamp(dot(frag_normal, frag_to_light), 0.0, 1.0);

    return light_color * oren_nayar(NdotL, NdotV, roughness);
}
*/