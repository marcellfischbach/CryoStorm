program {
    shaders {
        shader "directional_light_deferred_shadow.frag",
        shader "directional_light_deferred.vert",
    },
    attributes {
        attribute CameraPosition,
        attribute LightColor,
        attribute LightAmbientColor,
        attribute NegLightDirection,
        attribute DiffuseRoughness,
        attribute Normal,
        attribute Depth,
        attribute ShadowMap,
        attribute PartialRect,
    }
}