package org.crimsonedge.launcher;

import org.crimsonedge.core.IGame;

public class LauncherGame implements IGame {

    @Override
    public boolean initialize() {
        System.out.println("LauncherGame.initialize");
        return true;
    }
}
