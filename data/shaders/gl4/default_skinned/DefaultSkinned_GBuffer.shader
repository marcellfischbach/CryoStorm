program {
    shaders {
        shader "DefaultSkinned_GBuffer.vert",
        shader "../default/Default_GBuffer.frag",
    }
    attributes {
        attribute "Diffuse",
        attribute "Normal",
        attribute "RoughnessMap",
        attribute "Roughness",
        attribute "Metallic",
        attribute "Color",
    }
}