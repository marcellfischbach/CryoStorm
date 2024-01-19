#version 330

layout(location = eVS_Vertices) in vec3 ce_Position;
layout(location = eVS_UV) in vec2 ce_TexCoord;

uniform vec2 ce_RectMin;
uniform vec2 ce_RectMax;

out vec2 texCoord;
out vec2 ndc;

void main ()
{
    // position is in range -1..+1

    vec2 fact = ce_Position.xy * 0.5 + 0.5;
    // fact is in range 0..1


    // RectMax/RectMin in range 0..1
    vec2 delta = ce_RectMax - ce_RectMin;


    vec2 pos = ce_RectMin + fact * delta;



    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);



    texCoord = ce_RectMin + ce_TexCoord * delta;
    // tecCoord is in range 0..1



    ndc = texCoord * 2.0 - 1.0;

}

