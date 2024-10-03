#version 330

layout(location = eVS_Vertices) in vec3 cs_Position;
layout(location = eVS_UV) in vec2 cs_TexCoord;

uniform vec2 cs_RectMin;
uniform vec2 cs_RectMax;

out vec2 texCoord;
out vec2 ndc;

void main ()
{
    // position is in range -1..+1

    vec2 fact = cs_Position.xy * 0.5 + 0.5;
    // fact is in range 0..1


    // RectMax/RectMin in range 0..1
    vec2 delta = cs_RectMax - cs_RectMin;


    vec2 pos = cs_RectMin + fact * delta;



    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);



    texCoord = cs_RectMin + cs_TexCoord * delta;
    // tecCoord is in range 0..1



    ndc = texCoord * 2.0 - 1.0;

}

