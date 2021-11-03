#define PI 3.14159265

vec3 oren_nayar (vec3 light_color, vec3 frag_to_light, vec3 frag_normal, vec3 frag_to_viewer, float roughness)
{

    float NdotV = clamp(dot(frag_normal, frag_to_viewer), 0.0, 1.0);
    float NdotL = clamp(dot(frag_normal, frag_to_light), 0.0, 1.0);
    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);

    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = cos(angleVN - angleLN);

    float roughness2 = roughness * roughness;
    float A = 1.0 - 0.5 * roughness2 / (roughness2 + 0.33);
    float B = 0.45 *      roughness2 / (roughness2 + 0.09);
    float C = sin(alpha) * tan(beta);
    float OrenNayar = NdotL * (A + (B * max(0.0, gamma) * C));
    return light_color * OrenNayar;
}



vec3 calc_lighting (vec3 light_color, vec3 frag_to_light, vec3 frag_normal, vec3 frag_to_viewer, float roughness)
{
    return oren_nayar(light_color, frag_to_light, frag_normal, frag_to_viewer, roughness);
}
