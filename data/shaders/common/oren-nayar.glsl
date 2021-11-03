
#define PI 3.14159265

vec3 calc_lighting (vec3 light_color, vec3 frag_to_light, vec3 frag_normal, vec3 frag_to_viewer, float roughness)
{
    float LdotV = dot(frag_to_light, frag_to_viewer);
    float NdotL = dot(frag_to_light, frag_normal);
    float NdotV = dot(frag_normal, frag_to_viewer);

    float s = LdotV - NdotL * NdotV;
    float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

    float sigma2 = roughness * roughness;
    float A = 1.0 - 0.5 * (sigma2/(((sigma2 + 0.33) + 0.000001)));
    float B = 0.45 * sigma2 / ((sigma2 + 0.09) + 0.00001);

    float ga = dot(frag_to_viewer-frag_normal*NdotV,frag_normal-frag_normal*NdotL);

    float influence = max(0., NdotL) * (A + B * max(0., ga) * sqrt(max((1.0-NdotV*NdotV)*(1.0-NdotL*NdotL), 0.)) / max(NdotL, NdotV));
    return influence * light_color;
}
