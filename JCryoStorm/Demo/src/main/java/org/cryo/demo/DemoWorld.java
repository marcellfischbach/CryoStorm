package org.cryo.demo;

import org.cryo.core.ObjectRegistry;
import org.cryo.core.entity.Entity;
import org.cryo.core.entity.TerrainLayer;
import org.cryo.core.entity.TerrainMeshState;
import org.cryo.core.entity.World;
import org.cryo.core.graphics.ETerrainSize;
import org.cryo.core.graphics.ITerrainMesh;
import org.cryo.core.graphics.ITerrainMeshGenerator;
import org.cryo.core.graphics.ITerrainMeshGeneratorFactory;
import org.cryo.core.math.Vector3f;
import org.cryo.core.resource.AssetManager;

import java.util.Arrays;

public class DemoWorld {

    private static ITerrainMesh createFlatTerrainMesh(float size) {
        ITerrainMeshGenerator generator = ObjectRegistry.get(ITerrainMeshGeneratorFactory.class).Create();

        float[] heightData = new float[1025*1025];
        Arrays.fill(heightData, 0.0f);

        generator.setSize(ETerrainSize.TS_129);
        generator.setPatchSize(ETerrainSize.TS_33);
        generator.setSize(ETerrainSize.TS_1025);
        generator.setPatchSize(ETerrainSize.TS_129);

        generator.setNormalizedHeightData(heightData);
        generator.setSize(new Vector3f(-size, 0.0f, -size), new Vector3f (size, 10.0f, size));

        return generator.generate();
    }

    private static void setupTerrain(World world) {
        AssetManager assetMan = AssetManager.instance();

        TerrainLayer greenGrassLayer = assetMan.get(TerrainLayer.class, "/terrain/green_grass.terrainlayer");
        TerrainLayer fieldstoneLayer = assetMan.get(TerrainLayer.class, "/terrain/fieldstone.terrainlayer");
        TerrainLayer terrainLayers = assetMan.get(TerrainLayer.class, "/terrain/terrain.terrainlayer");

        ITerrainMesh terrainMesh = createFlatTerrainMesh(40.0f);
        Entity entity0 = new Entity();
        TerrainMeshState terrainState = new TerrainMeshState ();
        terrainState.setTerrainMesh(terrainMesh);
        terrainState.setLayerMask(terrainLayers);
        terrainState.AddLayer(greenGrassLayer);
        terrainState.AddLayer(dirtLayer);
        terrainState.AddLayer(fieldstoneLayer);
        terrainState.GetTransform()
                .SetTranslation(cs::csVector3f (0, 0, 0))
              .Finish();
        terrainState -> SetStatic(true);
        entity0 -> AttachState(terrainState);
        world -> Attach(entity0);
    }

    public static void create(World world) {
        setupTerrain(world);
    }
}
