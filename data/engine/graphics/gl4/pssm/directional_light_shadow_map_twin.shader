program {
    shaders {
        shader "directional_light_shadow_map_twin.frag",
        shader "directional_light_shadow_map_twin.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowBuffers,
        attribute DepthBuffer,
    }
}