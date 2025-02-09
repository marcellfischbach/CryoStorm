#version 330
layout (location = 0) out vec2 cs_FragColor;

//uniform sampler2D cs_Diffuse;
//
//in vec2 texCoord;

void main()
{
    cs_FragColor.x = gl_FragDepth;

    // Compute partial derivatives of depth.
    float dx = dFdx(gl_FragDepth);
    float dy = dFdy(gl_FragDepth);
    // Compute second moment over the pixel extents.
    cs_FragColor.y = gl_FragDepth * gl_FragDepth + 0.25 * (dx * dx + dy * dy);
    //
//    float alpha = texture(cs_Diffuse, texCoord).a;
//    if (alpha < 0.5)
//    {
//        discard;
//    }
//
//    cs_FragColor = vec4(1, 1, 1, 1);
}



