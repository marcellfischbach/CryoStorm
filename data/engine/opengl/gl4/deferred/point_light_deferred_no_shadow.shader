program {
    shaders {
        shader "point_light_deferred_no_shadow.frag",
        shader "point_light_deferred.vert",
    },
    attributes {
        attribute CameraPosition,
        attribute LightColor,
        attribute LightAmbientColor,
        attribute LightPosition,
        attribute LightRange,
        attribute DiffuseRoughness,
        attribute Normal,
        attribute Depth
    }
}