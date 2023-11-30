program {
    shaders {
        shader "directional_light_shadow_map_alt.frag",
        shader "directional_light_shadow_map_alt.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowBuffers,
        attribute DepthBuffer,
    }
}