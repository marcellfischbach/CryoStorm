program {
    shaders {
        shader "directional_light_shadow_map.frag",
        shader "directional_light_shadow_map.vert",
    },
    attributes {
        uniform LayersBias,
        uniform MappingMatrices,
        uniform ShadowBuffer,
        uniform DepthBuffer,
    }
}