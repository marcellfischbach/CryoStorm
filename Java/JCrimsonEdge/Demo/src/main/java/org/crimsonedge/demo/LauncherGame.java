package org.crimsonedge.demo;

import org.crimsonedge.core.Engine;
import org.crimsonedge.core.IGame;
import org.crimsonedge.core.entity.csWorld;
import org.crimsonedge.demo.scene.TestEntity;

public class LauncherGame implements IGame {



    @Override
    public boolean initialize() {
        csWorld world = Engine.instance().getWorld();
        System.out.println("LauncherGame.initialize");



//        csEntity entity = new csEntity();
        world.attach(new TestEntity());


        System.out.println("LauncherGame.initialize - done");
        return true;
    }
}
