program {
    shaders {
        shader "point_light_shadow_map.frag",
        shader "point_light_shadow_map.vert",
    },
    attributes {
        attribute LightPosition,
        attribute MappingBias,
        attribute ShadowBuffer,
        attribute DepthBuffer,
        attribute PartialRect,

    }
}