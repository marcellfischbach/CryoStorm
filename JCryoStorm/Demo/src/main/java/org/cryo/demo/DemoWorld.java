package org.cryo.demo;

import org.cryo.core.CsObjectRegistry;
import org.cryo.core.entity.*;
import org.cryo.core.graphics.*;
import org.cryo.core.math.CsColor4f;
import org.cryo.core.math.CsQuaternion;
import org.cryo.core.math.CsVector3f;
import org.cryo.core.resource.CsAssetManager;

import java.util.Arrays;

public class DemoWorld {

    private static ITerrainMesh createFlatTerrainMesh(float size) {
        ITerrainMeshGenerator generator = CsObjectRegistry.get(ITerrainMeshGeneratorFactory.class).Create();

        float[] heightData = new float[1025 * 1025];
        Arrays.fill(heightData, 0.0f);

        generator.setSize(ETerrainSize.TS_129);
        generator.setPatchSize(ETerrainSize.TS_33);
        generator.setSize(ETerrainSize.TS_1025);
        generator.setPatchSize(ETerrainSize.TS_129);

        generator.setNormalizedHeightData(heightData);
        generator.setSize(new CsVector3f(-size, 0.0f, -size), new CsVector3f(size, 10.0f, size));

        return generator.generate();
    }

    private static void setupTerrain(CsWorld world) {
        CsAssetManager assetMan = CsAssetManager.instance();

        CsTerrainLayer greenGrassLayer = assetMan.get(CsTerrainLayer.class, "/terrain/green_grass.terrainlayer");
        CsTerrainLayer fieldstoneLayer = assetMan.get(CsTerrainLayer.class, "/terrain/fieldstone.terrainlayer");
        CsTerrainLayerMask terrainLayers = assetMan.get(CsTerrainLayerMask.class, "/terrain/terrain.terrainmask");
        CsTerrainLayer dirtLayer = assetMan.get(CsTerrainLayer.class, "/terrain/dirt.terrainlayer");

        ITerrainMesh terrainMesh = createFlatTerrainMesh(40.0f);
        CsEntity entity0 = new CsEntity();
        CsTerrainMeshState terrainState = new CsTerrainMeshState();
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

    private static void setupCamera(CsWorld world) {
        CsEntity cameraEntity = new CsEntity();
        CsCameraState cameraState = new CsCameraState();
        cameraState.setClearMode(EClearMode.DEPTH_COLOR);
        cameraState.setClearColor(new CsColor4f(0.0f, 0.0f, 0.0f, 1.0f));
        cameraState.setClearColorMode(EClearColorMode.PLAIN_COLOR);

        cameraEntity.attachState(cameraState);
//        cameraEntity.attachState(new CameraHandler());
        cameraEntity.attachState(new CameraHandlerComponent());
        world.attach(cameraEntity);
        world.setMainCamera(cameraState);


    }

    private static CsLightState addDirectionalLight(CsWorld world,
                                                    CsVector3f axis,
                                                    float rad,
                                                    CsColor4f color,
                                                    boolean isStatic,
                                                    boolean castsShadow
                                                 ) {
        CsEntity entity = new CsEntity();
        CsLightState lightState = new CsLightState();
        entity.attachState(lightState);
        lightState.setType(ELightType.DIRECTIONAL);
        lightState.setColor(color);
        lightState.setShadowMapBias(0.003f);
        lightState.setStatic(isStatic);
        lightState.setCastShadow(castsShadow);
        lightState.getTransform()
                .setRotation(CsQuaternion.fromAxisAngle(axis.normalize(), rad))
                .finish();
        world.attach(entity);
        return lightState;
    }


    private static void setupLight(CsWorld world) {
        addDirectionalLight(world,
                            new CsVector3f(1.0f, 0.2f, 0.0f),
                            -45.0f * (float) Math.PI / 180.0f,
                            new CsColor4f(1.0f, 1.0f, 1.0f, 1.0f).mul(0.9f),
                            true,
                            true);
    }

    public static void create(CsWorld world) {
        setupCamera(world);
        setupTerrain(world);
        setupLight(world);
    }
}
