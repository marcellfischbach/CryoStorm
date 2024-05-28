#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

out vec3 uv;
out vec4 color;

void main ()
{
    //
    // PosX
    gl_Layer = 0;
    color = vec4(1, 0.5, 0.5, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();


    //
    // NegX
    gl_Layer = 1;
    color = vec4(0, 0.5, 0.5, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();


    //
    // PosY
    gl_Layer = 2;
    color = vec4(0.5, 1, 0.5, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();


    //
    // NegY
    gl_Layer = 3;
    color = vec4(0.5, 0, 0.5, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();

    //
    // PosZ
    gl_Layer = 4;
    color = vec4(0.5, 0.5, 1, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();


    //
    // NegZ
    gl_Layer = 5;
    color = vec4(0.5, 0.5, 0, 1);

    gl_Position = vec4(-1.0, -1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, -1.0, 0.0, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    gl_Position = vec4(1.0, 1.0, 0.0, 1.0f);
    EmitVertex();
    EndPrimitive();




}