program {
    shaders {
        shader "../pp_standard.vert",
        shader "blur_v.frag",
    }
    attributes {
        attribute "Color",
        attribute "TextureSizeInv",
        attribute "SampleScale",
        attribute "SampleCount"
    }
}