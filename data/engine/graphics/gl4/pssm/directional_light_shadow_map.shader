program {
    shaders {
        shader "directional_light_shadow_map.frag",
        shader "directional_light_shadow_map.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowBuffers,
        attribute DepthBuffer,
    }
}