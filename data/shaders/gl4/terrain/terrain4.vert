#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_Normals) in vec3 cs_Normal;
layout(location = eVS_Colors) in vec4 cs_Color;
layout(location = eVS_UV) in vec2 cs_UV;


uniform mat4 cs_ModelMatrix;
uniform mat4 cs_ViewMatrix;
uniform mat4 cs_ViewMatrixInv;
uniform mat4 cs_ViewProjectionMatrix;

out vec4 color;
out vec2 texCoord;
out vec3 world_position;
out vec3 world_normal;
out vec3 camera_space_position;
out vec3 viewer_world_position;
out vec2 screen_coord;

void main()
{
    vec4 position = cs_ModelMatrix * cs_Position;
    world_position = position.xyz;
    world_normal = (cs_ModelMatrix * vec4(cs_Normal, 0.0)).xyz;

    viewer_world_position = (cs_ViewMatrixInv * vec4(0, 0, 0, 1)).xyz;
    camera_space_position = (cs_ViewMatrix * position).xyz;

    gl_Position = cs_ViewProjectionMatrix * position;
    color = cs_Color;
    texCoord = cs_UV;
    screen_coord = (gl_Position.xy / gl_Position.w) * 0.5 + 0.5;

}

