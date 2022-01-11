material {
    shaders {
        shader Forward "/shaders/test_color_unlit_program.spc",
        shader ShadowCube "/shaders/test_shadow_point_program.spc",
        shader ShadowPSSM "/shaders/test_shadow_pssm_program.spc",
        shader Shadow "/shaders/test_shadow_program.spc"
    },
    attributes {
        attribute Texture "Diffuse" "/textures/grass_texture.spc",
        attribute Color4 "Color" 1 1 1 1,
    }
}