package org.crimsonedge.launcher;

import org.crimsonedge.core.Engine;
import org.lwjgl.opengl.GL;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;
import java.awt.*;

public class LwjglWindow extends AWTGLCanvas {

    private String[] args;

    private static native long nInitWindow (LwjglWindow window);

    private long wnd;

    public LwjglWindow(String[] args) {
        super(glData());
        this.wnd = nInitWindow(this);

        this.args = args;
        Dimension size = new Dimension(1024, 768);
        setMinimumSize(size);
        setMaximumSize(size);
        setPreferredSize(size);

        Runnable renderLoop = new Runnable() {
            @Override
            public void run() {
                if (!isValid()) {
                    return;
                }
                render();
                SwingUtilities.invokeLater(this);
            }
        };
        SwingUtilities.invokeLater(renderLoop);
    }

    public int getWindowWidth () {
        return getWidth();
    }

    public int getWindowHeight () {
        return getHeight();
    }

    @Override
    public void initGL() {
        System.out.println("LwjglWindow.initGL");
        Engine.instance().initialize(this.args, new LauncherGame());
    }

    @Override
    public void paintGL() {
        Engine.instance().processFrame();

        swapBuffers();
    }



    private static GLData glData() {
        GLData data = new GLData();
        data.majorVersion = 4;
        data.minorVersion = 4;
        data.profile = GLData.Profile.CORE;
        data.swapInterval = 0;
        return data;
    }
}
