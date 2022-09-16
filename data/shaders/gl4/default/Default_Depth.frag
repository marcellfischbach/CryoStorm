#version 330

uniform sampler2D ce_Diffuse;

in vec2 texCoord;

void main()
{

    float alpha = texture(ce_Diffuse, texCoord).a;
    if (alpha < 0.5)
    {
        discard;
    }

}



