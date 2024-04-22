program {
    shaders {
        shader "../pp_standard.vert",
        shader "high_pass.frag",
    }
    attributes {
        attribute "Color",
        attribute "HighValue"
    }
}