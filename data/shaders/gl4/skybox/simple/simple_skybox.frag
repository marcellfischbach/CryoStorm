#version 330
layout (location = 0)  out vec4 ce_FragColor;


in vec3 fs_normal;
in vec4 fs_color;




void main ()
{
    //
    // Configurations.. should be passed as arguments later
    float ce_Hi = 1.0;
    float ce_Med = 0.0;
    float ce_Low = -0.3;
    vec4 ce_HiColor = vec4(0.25, 0.75, 1.0, 1.0);
    vec4 ce_MedColor = vec4(0.35, 0.75, 0.9, 1.0);
    vec4 ce_LowColor = vec4(0.75, 0.75, 0.75, 1.0);


    float ce_SunDisk = 0.0005;
    float ce_SunBleedOut = ce_SunDisk + 0.0005;
    vec3 ce_SunDirection = -vec3 (0.14, -0.69, 0.71);
    vec4 ce_SunColor = vec4(1.0, 1.0, 1.0, 1.0);


    //
    // real calculations from here on
    vec3 norm = normalize(fs_normal);
    float sunDisk = 1.0 - ce_SunDisk;
    float sunBleedOut = 1.0 - ce_SunBleedOut;
    vec3 sunDirection = normalize(ce_SunDirection);

    //
    // calculate the background color
    vec4 backgroundColor;
    {
        float value = clamp(dot(norm, vec3(0, 1, 0)), ce_Low, ce_Hi);

        vec4 color;
        if (value >= ce_Med) {
            value = (value - ce_Med) / (ce_Hi - ce_Med);
            backgroundColor = mix (ce_MedColor, ce_HiColor, value);
        }
        else {
            value = (value - ce_Low)  / (ce_Med - ce_Low);
            backgroundColor = mix (ce_LowColor, ce_MedColor, value);
        }
    }


    // calculate the sun factor
    float sunFactor = 0.0;
    {
        float value = dot (norm, sunDirection);

        sunFactor = smoothstep(sunBleedOut, sunDisk, value);
    }




    ce_FragColor = mix(backgroundColor, ce_SunColor, sunFactor);
}