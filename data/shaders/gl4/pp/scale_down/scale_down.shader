program {
    shaders {
        shader "../pp_standard.vert",
        shader "scale_down.frag",
    }
    attributes {
        attribute "Color",
        attribute "TextureSizeInv"
    }
}