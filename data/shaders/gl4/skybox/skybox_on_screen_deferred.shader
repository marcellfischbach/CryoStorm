program {
    shaders {
        shader "skybox_on_screen_deferred.vert",
        shader "skybox_on_screen_deferred.frag",
    }
    attributes {
        attribute "Skybox",
        attribute "RenderPlane",
        attribute "Depth",
    }
}