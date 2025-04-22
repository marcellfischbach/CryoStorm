# Material

The material file format is a text file in the CSF (CryoStormFile) format.
The extension of a file material file is **.mat** and the root element is **material**.

As an example this is the default material of the engine.

``` CSF
material {
    depth Test Write,
    shaders {
        shader Forward    "../shaders/${rendering-api}/default/Default.shader",
        shader Depth      "../shaders/${rendering-api}/default/Default_Depth.shader",
        shader GBuffer    "../shaders/${rendering-api}/default/Default_GBuffer.shader",
        shader ShadowCube "../shaders/${rendering-api}/default/Default_PointShadow.shader",
        shader ShadowPSSM "../shaders/${rendering-api}/default/Default_PSSM.shader",
        shader Shadow     "../shaders/${rendering-api}/default/Default_Shadow.shader",
        shader VSM        "../shaders/${rendering-api}/default/Default_VSM.shader",
    },
    attributes {
        attribute Texture "Diffuse",
        attribute Texture "Normal" "textures/DefaultNormal.tex2d",
        attribute Texture "RoughnessMap" "textures/DefaultRoughness.tex2d",
        attribute Color4 "Color" 0 0 0 0,
        attribute Float "Roughness" 0.5,
        attribute Float "Metallic" 0.0,
        attribute Int "ReceiveShadow" 1,
    }
}
```

The base structure of the file is as follows:

``` CSF
material {
    shading,
    queue,
    blend,
    depth,
    shaders {
        shader,
        ...
    },
    attributes {
        attribute,
        ...
    }
}
```

## The file stages

### Shading

``shading <shading>,``

| `<shading>`        | Description                                                                                                                  |
|--------------------|------------------------------------------------------------------------------------------------------------------------------|
| `Unshaded`         | The material is rendered at the end of the rendering phase and is not lit. Unshaded objects are not rendered to the GBuffer. |
| `Shaded` (default) | The material is lit in forward pass and should be rendered to the GBuffer in deferred if it is not transparent               |                                                                                                                          

### Queue

```queue <queue>,```

| `<queue>`            | Description                                                                                                                                                                                                 |
|----------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `Default`  (default) | The material is a __normal__ material for solid objects. It is rendered to the depth-buffer and is present in the GBufferr                                                                                  |
| `Transparency`       | The material is a __transparent__ material and will be blended on top of other objects. Not present in the depth-buffer and not rendered to the GBuffer. Lighting must be performed in forward pass always. |

### Serp

```blend <type>,```

or

```blend <src> <dst>,```

or

```blend <src-color> <src-alpha> <dst-color> <dst-alpha>,```

| `<type>` | Description                                                                                                                 |
|----------|-----------------------------------------------------------------------------------------------------------------------------|
| `Serp`  | Normal blending. The object is blended with the background. The higher the alpha value, the less is the background visible. |
| `Add`    | The material is not blended but just adding to the color buffer                                                             |

For `<src>`, `<dst>`, `<src-color>`,`<src-alpha>`, `<dst-color>` and `<dst-alpha>`. One of the following values can be
used.

| Value              | Description |
|--------------------|-------------|
| `One`              |             |
| `Zero`             |             |
| `SrcColor`         |             |
| `SrcAlpha`         |             |
| `DstColor`         |             |
| `DstAlpha`         |             |
| `OneMinusSrcColor` |             |
| `OneMinusSrcAlpha` |             |
| `OneMinusDstColor` |             |
| `OneMinusDstAlpha` |             |