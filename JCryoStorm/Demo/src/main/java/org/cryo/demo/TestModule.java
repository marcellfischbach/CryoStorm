package org.cryo.demo;

import org.cryo.core.CsEngine;
import org.cryo.core.CsJavaModule;

public class TestModule extends CsJavaModule
{

    @Override
    public boolean register(String[] args, CsEngine engine) {
        return true;
    }

    @Override
    public boolean initialize(String[] args, CsEngine engine) {
        return true;
    }
}
