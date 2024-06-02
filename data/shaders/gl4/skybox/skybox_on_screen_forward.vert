#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_UV) in vec3 ce_UV;

uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ProjectionMatrix;
uniform float ce_RenderPlane;

out vec3 uv;

void main ()
{
    vec3 pos = mat3(ce_ViewMatrix) * ce_Position.xyz * ce_RenderPlane;
    gl_Position = ce_ProjectionMatrix * vec4(pos, 1.0);

    uv = ce_UV;
}