package org.crimsonedge.launcher;

import org.crimsonedge.core.Engine;
import org.crimsonedge.core.IGame;
import org.crimsonedge.core.entity.StaticMeshState;
import org.crimsonedge.core.graphics.IRenderMesh;
import org.crimsonedge.core.graphics.IRenderMeshGenerator;
import org.crimsonedge.core.graphics.IRenderMeshGeneratorFactory;
import org.crimsonedge.core.ObjectRegistry;
import org.crimsonedge.core.entity.Entity;
import org.crimsonedge.core.entity.World;
import org.crimsonedge.core.graphics.Mesh;
import org.crimsonedge.core.graphics.material.IMaterial;
import org.crimsonedge.core.math.Vector2f;
import org.crimsonedge.core.math.Vector3f;
import org.crimsonedge.core.math.Vector4f;
import org.crimsonedge.core.resource.AssetManager;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class LauncherGame implements IGame {



    @Override
    public boolean initialize() {
        World world = Engine.instance().getWorld();
        System.out.println("LauncherGame.initialize");



//        Entity entity = new Entity();
        world.attach(new TestEntity());


        System.out.println("LauncherGame.initialize - done");
        return true;
    }
}
