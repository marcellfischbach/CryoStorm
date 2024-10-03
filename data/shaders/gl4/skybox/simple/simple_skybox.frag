#version 330
layout (location = 0)  out vec4 cs_FragColor;


in vec3 fs_normal;
in vec4 fs_color;




void main ()
{
    //
    // Configurations.. should be passed as arguments later
    float cs_Hi = 1.0;
    float cs_Med = 0.0;
    float cs_Low = -0.3;
    vec4 cs_HiColor = vec4(0.25, 0.75, 1.0, 1.0);
    vec4 cs_MedColor = vec4(0.35, 0.75, 0.9, 1.0);
    vec4 cs_LowColor = vec4(0.75, 0.75, 0.75, 1.0);


    float cs_SunDisk = 0.0005;
    float cs_SunBleedOut = cs_SunDisk + 0.0005;
    vec3 cs_SunDirection = -vec3 (0.14, -0.69, 0.71);
    vec4 cs_SunColor = vec4(1.0, 1.0, 1.0, 1.0);


    //
    // real calculations from here on
    vec3 norm = normalize(fs_normal);
    float sunDisk = 1.0 - cs_SunDisk;
    float sunBleedOut = 1.0 - cs_SunBleedOut;
    vec3 sunDirection = normalize(cs_SunDirection);

    //
    // calculate the background color
    vec4 backgroundColor;
    {
        float value = clamp(dot(norm, vec3(0, 1, 0)), cs_Low, cs_Hi);

        vec4 color;
        if (value >= cs_Med) {
            value = (value - cs_Med) / (cs_Hi - cs_Med);
            backgroundColor = mix (cs_MedColor, cs_HiColor, value);
        }
        else {
            value = (value - cs_Low)  / (cs_Med - cs_Low);
            backgroundColor = mix (cs_LowColor, cs_MedColor, value);
        }
    }


    // calculate the sun factor
    float sunFactor = 0.0;
    {
        float value = dot (norm, sunDirection);

        sunFactor = smoothstep(sunBleedOut, sunDisk, value);
    }




    cs_FragColor = mix(backgroundColor, cs_SunColor, sunFactor);
}