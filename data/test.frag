#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;

uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];

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
            * (1.0 - distance / light_range);
}

vec4 calc_directional_light(vec4 light_color, vec3 light_vector, vec3 frag_normal)
{
  return light_color * dot(light_vector, frag_normal);
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
  frag_color += calc_light(spc_LightColor[0], spc_LightVector[0], spc_LightRange[0], world_position, norm) * color;
  frag_color += calc_light(spc_LightColor[1], spc_LightVector[1], spc_LightRange[1], world_position, norm) * color;
  frag_color += calc_light(spc_LightColor[2], spc_LightVector[2], spc_LightRange[2], world_position, norm) * color;
  frag_color += calc_light(spc_LightColor[3], spc_LightVector[3], spc_LightRange[3], world_position, norm) * color;

  spc_FragColor = frag_color * texture(spc_Diffuse, texCoord);
//  spc_FragColor = vec4(norm * 0.5 + 0.5, 1.0);
  //spc_FragColor = color;
}

