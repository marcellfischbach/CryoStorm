package org.crimsonedge.launcher;

import org.crimsonedge.core.Engine;
import org.crimsonedge.core.IGame;
import org.crimsonedge.core.graphics.IRenderMeshGeneratorFactory;
import org.crimsonedge.core.ObjectRegistry;
import org.crimsonedge.core.entity.Entity;
import org.crimsonedge.core.entity.World;

public class LauncherGame implements IGame {

    @Override
    public boolean initialize() {
        World world = Engine.instance().getWorld();
        System.out.println("LauncherGame.initialize");


        IRenderMeshGeneratorFactory iRenderMeshGeneratorFactory = ObjectRegistry.get(IRenderMeshGeneratorFactory.class);

        Entity entity = new Entity();
        entity.attach(new MyState());
        world.attach(entity);


        System.out.println("LauncherGame.initialize - done");
        return true;
    }
}
