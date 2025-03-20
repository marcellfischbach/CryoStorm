


float cook_torrance(float F0, float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_v, float roughness)
{
	float Rs = 0.0;
	if (n_dot_l > 0) 
	{

		// Fresnel reflectance
		float F = pow(1.0 - h_dot_v, 5.0);
		F *= (1.0 - F0);
		F += F0;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(n_dot_h, 4.0));
		float r2 = (n_dot_h * n_dot_h - 1.0) / (m_squared * n_dot_h * n_dot_h);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_n_dot_h = 2.0 * n_dot_h;
		float g1 = (two_n_dot_h * n_dot_v) / h_dot_v;
		float g2 = (two_n_dot_h * n_dot_l) / h_dot_v;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (3.14159265 * n_dot_l * n_dot_v);
	}
	float k = 0.0;
	return clamp(n_dot_l * (k + Rs * (1.0 - k)), 0.0, 1.0);
}