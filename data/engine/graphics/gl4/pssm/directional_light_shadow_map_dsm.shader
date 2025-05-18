program {
    shaders {
        shader "directional_light_shadow_map_dsm.frag",
        shader "directional_light_shadow_map_dsm.vert",
    },
    attributes {
        attribute LayersDepth,
        attribute LayersBias,
        attribute ShadowDepthRange,
        attribute ShadowBuffers,
        attribute ShadowBufferDatas,
        attribute ShadowBufferPixelSize,
        attribute ShadowBufferDepthBias,
        attribute BlurFactor,
        attribute DepthBuffer,
        attribute PartialRect,
    }
}