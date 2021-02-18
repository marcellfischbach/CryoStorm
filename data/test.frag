#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;

uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCount;

in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;

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

vec4 calc_light(vec4 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal)
{
    if (light_vector.w == 1.0)
    {
        return calc_point_light(light_color, light_vector.xyz, light_range, frag_position, frag_normal);
    }
    else
    {
        return calc_directional_light(light_color, light_vector.xyz, frag_normal);
    }
}

void main()
{
    vec3 norm = normalize(world_normal);
    vec4 frag_color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < spc_LightCount; i++)
    {
      frag_color += calc_light(spc_LightColor[i], spc_LightVector[i], spc_LightRange[i], world_position, norm);
    }
    frag_color *= color;
    frag_color *= texture(spc_Diffuse, texCoord);
    spc_FragColor = frag_color;
    //  spc_FragColor = vec4(norm * 0.5 + 0.5, 1.0);
    //spc_FragColor = color;
}

