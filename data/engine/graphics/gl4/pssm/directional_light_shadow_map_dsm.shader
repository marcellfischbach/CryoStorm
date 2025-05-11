program {
    shaders {
        shader "directional_light_shadow_map_dsm.frag",
        shader "directional_light_shadow_map_dsm.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowBuffers,
        attribute ShadowBufferDatas,
        attribute DepthBuffer,
        attribute PartialRect,
    }
}