program {
    shaders {
        shader "directional_light_renderer.frag",
        shader "directional_light_renderer.vert",
    },
    attributes {
        attribute DiffuseRoughness,
        attribute Depth,
        attribute Normal,
    }
}