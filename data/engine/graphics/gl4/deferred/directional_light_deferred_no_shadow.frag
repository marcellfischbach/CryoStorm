#version 330

layout (location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_DiffuseRoughness;
uniform sampler2D cs_Normal;
uniform sampler2D cs_Depth;

uniform mat4 cs_ViewProjectionMatrixInv;

uniform vec3 cs_CameraPosition;
uniform vec4 cs_LightColor;
uniform vec4 cs_LightAmbientColor;
uniform vec3 cs_NegLightDirection;

in vec2 texCoord;

in vec2 ndc;


#include</shaders/gl4/common/cook-torrance.glsl>
#include</shaders/gl4/common/oren-nayar.glsl>




void main ()
{
    //
    // decompose the world position
    float z = texture(cs_Depth, texCoord).r;
    if (z == 1.0)
    {
        discard;
    }
    vec4 worldPosition = vec4(texCoord, z, 1.0);
    worldPosition = worldPosition * 2.0 - 1.0;

    worldPosition = cs_ViewProjectionMatrixInv * worldPosition;
    worldPosition /= worldPosition.w;
    // coord now holds the world coordinate


    //
    // decompose the world normal
    vec3 normal = texture(cs_Normal, texCoord).xyz;
    normal = normal * 2.0 - 1.0;
    // normal now holds the world normal

    vec4 diffuseRoughness = texture(cs_DiffuseRoughness, texCoord);

    vec3 to_viewer = normalize(cs_CameraPosition - worldPosition.xyz);

    vec3 light_dir = normalize(cs_NegLightDirection.xyz);

    vec3 H = normalize(light_dir + to_viewer);
    float n_dot_l = clamp(dot (normal, light_dir), 0, 1);
    float n_dot_v = clamp(dot (normal, to_viewer), 0, 1);
    float n_dot_h = clamp(dot(normal, H), 0, 1);
    float h_dot_l = clamp(dot(H, light_dir), 0, 1);
    float specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_l, diffuseRoughness.a);
    float diffuse = oren_nayar(n_dot_l, n_dot_v, diffuseRoughness.a);
    vec3 color = diffuseRoughness.rgb;


    cs_FragColor = vec4(
        color * diffuse * cs_LightColor.rgb +
        specular * cs_LightColor.rgb +
        cs_LightAmbientColor.rgb,
        1.0);



}

