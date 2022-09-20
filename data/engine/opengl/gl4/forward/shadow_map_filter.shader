program{
    shaders {
        shader "shadow_map_filter.frag",
        shader "shadow_map_filter.vert",
    },
    attributes {
        attribute ShadowMap,
        attribute DepthBuffer,
        attribute FilterRadius,
        attribute FilterSamples,
        attribute FilterDistance,
        attribute ScreenAspect,
    }
}