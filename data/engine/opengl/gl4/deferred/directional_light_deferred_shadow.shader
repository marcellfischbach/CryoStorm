program {
    shaders {
        shader "directional_light_deferred_shadow.frag",
        shader "directional_light_deferred.vert",
    },
    attributes {
        attribute CameraPosition,
        attribute LightColor,
        attribute NegLightDirection,
        attribute DiffuseRoughness,
        attribute Depth,
        attribute Normal,
    }
}