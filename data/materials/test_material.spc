material {
    shaders {
        shader "Forward" "/shaders/test_color_program.spc",
        shader "ShadowCube" "/shaders/test_shadow_point_program.spc",
        shader "ShadowPSSM" "/shaders/test_shadow_pssm_program.spc",
        shader "Shadow" "/shaders/test_shadow_program.spc"
    },
    attributes {
        texture "Diffuse" ,
        color "Color"
    }

}