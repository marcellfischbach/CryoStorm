program {
    shaders {
        shader "Default_GBuffer.vert",
        shader "Default_GBuffer.frag",
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