#version 330
layout (location = 0) out vec2 cs_FragColor;

//uniform sampler2D cs_Diffuse;
//
//in vec2 texCoord;

in vec2 depth;


void main()
{
    // this actually is gl_Position.z / gl_Position.w
    float fragDepth = depth.x / depth.y;
    fragDepth = fragDepth * 0.5 + 0.5;
//    float fragDepth = depth.x;
//    cs_FragColor.x = fragDepth;

    // Compute partial derivatives of depth.
    float dx = dFdx(fragDepth);
    float dy = dFdy(fragDepth);
    cs_FragColor.r = length(vec2(dx, dy));
//    cs_FragColor.r = sqrt(dx*dx +dy*dy);
//    cs_FragColor.y = fragDepth * fragDepth; // + 0.25 * (dx * dx + dy * dy);
    //
//    float alpha = texture(cs_Diffuse, texCoord).a;
//    if (alpha < 0.5)
//    {
//        discard;
//    }
//
//    cs_FragColor = vec4(1, 1, 1, 1);
}



