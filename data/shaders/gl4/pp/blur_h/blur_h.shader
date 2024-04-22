program {
    shaders {
        shader "../pp_standard.vert",
        shader "blur_h.frag",
    }
    attributes {
        attribute "Color",
        attribute "TextureSizeInv",
        attribute "SampleScale",
        attribute "SampleCount"
    }
}