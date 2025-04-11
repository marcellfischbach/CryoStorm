program {
    shaders {
        shader "Default-Skinned.vert",
        shader "../default/Default.frag",
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