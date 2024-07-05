
//
// lambert implementation of the diffuse lighting term
float calculate_diffuse_lighting (float n_dot_l, float n_dot_v, float roughness)
{
    return clamp(n_dot_l, 0.0, 1.0);
}

