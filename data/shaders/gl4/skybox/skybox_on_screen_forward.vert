#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_UV) in vec3 cs_UV;

uniform mat4 cs_ViewMatrix;
uniform mat4 cs_ProjectionMatrix;
uniform float cs_RenderPlane;

out vec3 uv;

void main ()
{
    vec3 pos = mat3(cs_ViewMatrix) * cs_Position.xyz * cs_RenderPlane;
    gl_Position = cs_ProjectionMatrix * vec4(pos, 1.0);

    uv = cs_UV;
}