material {
    shaders {
        shader Forward "/shaders/test_mirror_program.spc",
        shader ShadowCube "/shaders/test_shadow_point_program.spc",
        shader ShadowPSSM "/shaders/test_shadow_pssm_program.spc",
        shader Shadow "/shaders/test_shadow_program.spc"
    },
    attributes {
        attribute Texture "Diffuse" "/textures/grass_texture.spc",
        attribute Color4 "Color" 1 1 1 1,
        attribute Float "Roughness" 1.0,
        attribute Texture "Mirror"
    }
}