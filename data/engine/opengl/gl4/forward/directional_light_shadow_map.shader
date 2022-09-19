program {
    shaders {
        shader "directional_light_shadow_map.frag",
        shader "directional_light_shadow_map.vert",
    },
    attributes {
        attribute LayersBias,
        attribute MappingMatrices,
        attribute ShadowBuffer,
        attribute DepthBuffer,
    }
}