package org.crimsonedge.demo;

import org.lwjgl.opengl.GL;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;

public class TestCanvas extends AWTGLCanvas {

    public TestCanvas() {
        super(glData());

        Runnable loop = new Runnable() {
            @Override
            public void run() {
                render();

                SwingUtilities.invokeLater(this);
            }
        };
        SwingUtilities.invokeLater(loop);
    }

    @Override
    public void initGL() {
        GL.createCapabilities();
    }

    @Override
    public void paintGL() {
        System.out.println("Time: " + System.currentTimeMillis());
        GL11.glViewport(0, 0, getWidth(), getHeight());
        GL11.glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);

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
