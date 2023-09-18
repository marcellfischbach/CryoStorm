#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_Normals) in vec3 ce_Normal;
layout(location = eVS_Colors) in vec4 ce_Color;
layout(location = eVS_UV) in vec2 ce_UV;


uniform mat4 ce_ModelMatrix;
uniform mat4 ce_ModelViewProjectionMatrix;

out vec2 texCoord;
out vec3 world_normal;

void main()
{
    world_normal = (ce_ModelMatrix * vec4(ce_Normal, 0.0)).xyz;


    gl_Position = ce_ModelViewProjectionMatrix * ce_Position;
    texCoord = ce_UV;

}

