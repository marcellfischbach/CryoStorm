package org.crimsonedge.demo;

import org.crimsonedge.core.Engine;
import org.crimsonedge.core.IGame;
import org.crimsonedge.core.entity.World;
import org.crimsonedge.demo.scene.TestEntity;

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
