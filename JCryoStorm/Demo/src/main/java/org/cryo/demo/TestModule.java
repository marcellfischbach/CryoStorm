package org.cryo.demo;

import org.cryo.core.Engine;
import org.cryo.core.JavaModule;

public class TestModule extends JavaModule  {

    @Override
    public boolean register(String[] args, Engine engine) {
        return true;
    }

    @Override
    public boolean initialize(String[] args, Engine engine) {
        return true;
    }
}
