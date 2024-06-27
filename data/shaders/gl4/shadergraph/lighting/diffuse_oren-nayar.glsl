
//
// oren-nayar implementation of the diffuse lighting term
float calculate_diffuse_lighting (float n_dot_l, float n_dot_v, float roughness)
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
    return clamp(n_dot_l * (A + (B * max(0.0, gamma) * C)), 0.0, 1.0);
}
