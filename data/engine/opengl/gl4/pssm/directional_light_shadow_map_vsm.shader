program {
    shaders {
        shader "directional_light_shadow_map_vsm.frag",
        shader "directional_light_shadow_map_vsm.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowBuffers,
        attribute ShadowBufferDatas,
        attribute DepthBuffer,
    }
}