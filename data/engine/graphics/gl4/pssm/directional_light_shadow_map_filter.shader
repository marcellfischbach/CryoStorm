program{
    shaders {
        shader "directional_light_shadow_map_filter.frag",
        shader "directional_light_shadow_map_filter.vert",
    },
    attributes {
        attribute ShadowMap,
        attribute DepthBuffer,
        attribute FilterRadius,
        attribute FilterSamplesFactor,
        attribute FilterDistance,
        attribute FilterMaxSampleDistance,
        attribute ScreenAspect,
        attribute ScreenSize,
    }
}