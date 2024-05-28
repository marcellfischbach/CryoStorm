#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_UV) in vec3 ce_UV;

uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ProjectionMatrix;

out vec3 uv;

void main ()
{
    vec4 center = ce_ViewMatrix * vec4(0, 0, 0, 1);
    vec4 pos -= center.xyz;
    gl_Position = ce_ProjectionMatrix * ce_ViewMatrixce_Position.xyz * 1), 1.0);
    uv = ce_UV;
}