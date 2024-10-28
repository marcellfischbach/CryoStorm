package org.cryo.demo;

import org.cryo.core.ObjectRegistry;
import org.cryo.core.entity.*;
import org.cryo.core.graphics.*;
import org.cryo.core.math.Color4f;
import org.cryo.core.math.Quaternion;
import org.cryo.core.math.Vector3f;
import org.cryo.core.resource.AssetManager;

import java.util.Arrays;

public class DemoWorld {

    private static ITerrainMesh createFlatTerrainMesh(float size) {
        ITerrainMeshGenerator generator = ObjectRegistry.get(ITerrainMeshGeneratorFactory.class).Create();

        float[] heightData = new float[1025 * 1025];
        Arrays.fill(heightData, 0.0f);

        generator.setSize(ETerrainSize.TS_129);
        generator.setPatchSize(ETerrainSize.TS_33);
        generator.setSize(ETerrainSize.TS_1025);
        generator.setPatchSize(ETerrainSize.TS_129);

        generator.setNormalizedHeightData(heightData);
        generator.setSize(new Vector3f(-size, 0.0f, -size), new Vector3f(size, 10.0f, size));

        return generator.generate();
    }

    private static void setupTerrain(World world) {
        AssetManager assetMan = AssetManager.instance();

        TerrainLayer greenGrassLayer = assetMan.get(TerrainLayer.class, "/terrain/green_grass.terrainlayer");
        TerrainLayer fieldstoneLayer = assetMan.get(TerrainLayer.class, "/terrain/fieldstone.terrainlayer");
        TerrainLayerMask terrainLayers = assetMan.get(TerrainLayerMask.class, "/terrain/terrain.terrainmask");
        TerrainLayer dirtLayer = assetMan.get(TerrainLayer.class, "/terrain/dirt.terrainlayer");

        ITerrainMesh terrainMesh = createFlatTerrainMesh(40.0f);
        Entity entity0 = new Entity();
        TerrainMeshState terrainState = new TerrainMeshState();
        terrainState.setTerrainMesh(terrainMesh);
        terrainState.setLayerMask(terrainLayers);
        terrainState.addLayer(greenGrassLayer);
        terrainState.addLayer(dirtLayer);
        terrainState.addLayer(fieldstoneLayer);
        terrainState.getTransform()
                .setTranslation(0, 0, 0)
                .finish();
        terrainState.setStatic(true);
        entity0.attachState(terrainState);
        world.attach(entity0);
    }

    private static void setupCamera(World world) {
        Entity cameraEntity = new Entity();
        CameraState cameraState = new CameraState();
        cameraState.setClearMode(EClearMode.DEPTH_COLOR);
        cameraState.setClearColor(new Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        cameraState.setClearColorMode(EClearColorMode.PLAIN_COLOR);

        cameraEntity.attachState(cameraState);
        cameraEntity.attachState(new CameraHandler());
        world.attach(cameraEntity);
        world.setMainCamera(cameraState);


    }

    private static LightState addDirectionalLight(World world,
                                                  Vector3f axis,
                                                  float rad,
                                                  Color4f color,
                                                  boolean isStatic,
                                                  boolean castsShadow
                                                 ) {
        Entity entity = new Entity();
        LightState lightState = new LightState();
        entity.attachState(lightState);
        lightState.setType(ELightType.DIRECTIONAL);
        lightState.setColor(color);
        lightState.setShadowMapBias(0.003f);
        lightState.setStatic(isStatic);
        lightState.setCastShadow(castsShadow);
        lightState.getTransform()
                .setRotation(Quaternion.fromAxisAngle(axis.normalize(), rad))
                .finish();
        world.attach(entity);
        return lightState;
    }


    private static void setupLight(World world) {
        addDirectionalLight(world,
                            new Vector3f(1.0f, 0.2f, 0.0f),
                            -45.0f * (float) Math.PI / 180.0f,
                            new Color4f(1.0f, 1.0f, 1.0f, 1.0f).mul(0.9f),
                            true,
                            true);
    }

    public static void create(World world) {
        setupCamera(world);
        setupTerrain(world);
        setupLight(world);
    }
}
