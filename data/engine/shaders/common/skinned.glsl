

vec4 skinned_calc_position (vec4 v, mat4 m0, mat4 m1, mat4 m2, mat4 m3, vec4 f)
{
    return (m0 * v) * f.x
        + (m1 * v) * f.y
        + (m2 * v) * f.z
        + (m3 * v) * f.w;
}



vec3 skinned_calc_normal (vec3 v3, mat4 m0, mat4 m1, mat4 m2, mat4 m3, vec4 f)
{
    vec4 v = vec4(v3, 0.0);
    return ((m0 * v) * f.x
    + (m1 * v) * f.y
    + (m2 * v) * f.z
    + (m3 * v) * f.w).xyz;
}