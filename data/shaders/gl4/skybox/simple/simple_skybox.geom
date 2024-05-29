#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

in vec2 gs_uv[];


out vec3 fs_normal;
out vec4 fs_color;

void main ()
{
    //
    // PosX
    gl_Layer = 0;
    fs_color = vec4(1, 0.5, 0.5, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(1.0, -gs_uv[0].y, -gs_uv[0].x);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(1.0, -gs_uv[1].y, -gs_uv[1].x);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(1.0, -gs_uv[2].y, -gs_uv[2].x);
    EmitVertex();
    EndPrimitive();


    //
    // NegX
    gl_Layer = 1;
    fs_color = vec4(0, 0.5, 0.5, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(-1.0, -gs_uv[0].y, gs_uv[0].x);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(-1.0, -gs_uv[1].y, gs_uv[1].x);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(-1.0, -gs_uv[2].y, gs_uv[2].x);
    EmitVertex();
    EndPrimitive();

    //
    // PosY
    gl_Layer = 2;
    fs_color = vec4(0.5, 1, 0.5, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(gs_uv[0].x, 1.0, gs_uv[0].y);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(gs_uv[1].x, 1.0, gs_uv[1].y);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(gs_uv[2].x, 1.0, gs_uv[2].y);
    EmitVertex();
    EndPrimitive();


    //
    // NegY
    gl_Layer = 3;
    fs_color = vec4(0.5, 0, 0.5, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(gs_uv[0].x, -1.0, -gs_uv[0].y);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(gs_uv[1].x, -1.0, -gs_uv[1].y);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(gs_uv[2].x, -1.0, -gs_uv[2].y);
    EmitVertex();
    EndPrimitive();

    //
    // PosZ
    gl_Layer = 4;
    fs_color = vec4(0.5, 0.5, 1, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(gs_uv[0].x, -gs_uv[0].y, 1.0);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(gs_uv[1].x, -gs_uv[1].y, 1.0);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(gs_uv[2].x, -gs_uv[2].y, 1.0);
    EmitVertex();
    EndPrimitive();


    //
    // NegZ
    gl_Layer = 5;
    fs_color = vec4(0.5, 0.5, 0, 1);

    gl_Position = gl_in[0].gl_Position;
    fs_normal = vec3(-gs_uv[0].x, -gs_uv[0].y, -1.0);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    fs_normal = vec3(-gs_uv[1].x, -gs_uv[1].y, -1.0);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    fs_normal = vec3(-gs_uv[2].x, -gs_uv[2].y, -1.0);
    EmitVertex();
    EndPrimitive();


}