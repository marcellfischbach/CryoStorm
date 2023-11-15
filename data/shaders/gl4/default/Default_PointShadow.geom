#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 ce_ShadowMapViewProjectionMatrix[1];
uniform int ce_RenderLayer;


void main()
{
    gl_Layer = ce_RenderLayer;

    gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();

}

