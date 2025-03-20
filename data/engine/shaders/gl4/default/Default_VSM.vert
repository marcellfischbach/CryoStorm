#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
//layout(location = eVS_UV) in vec2 cs_UV;


uniform mat4 cs_ModelViewProjectionMatrix;

//out vec2 texCoord;

out vec2 depth;

void main()
{
    gl_Position = cs_ModelViewProjectionMatrix * cs_Position;

    depth = vec2(gl_Position.z, gl_Position.w);
//    texCoord = cs_UV;
 }

