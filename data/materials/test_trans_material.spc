material {
    queue Transparency,
    blend SrcAlpha OneMinusSrcAlpha,
    depth Test Write,
    shaders {
        shader Forward "/shaders/test_trans_program.spc",
    },
    attributes {
        attribute Texture "Diffuse" "/textures/grass_texture.spc",
        attribute Color4 "Color" 1 1 1 1
    }

}