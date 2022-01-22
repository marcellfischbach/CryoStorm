program {
  shaders {
    shader "impl/test_color_mirror.vert",
    shader "impl/test_color_mirror.frag",
  }
  attributes {
    attribute "Diffuse",
    attribute "Mirror",
    attribute "Color",
    attribute "Roughness"
  }
}