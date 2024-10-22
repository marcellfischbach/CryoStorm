package org.crimsonedge.demo;

import org.cryo.core.ObjectRegistry;
import org.cryo.core.Viewport;
import org.cryo.core.entity.CameraState;
import org.cryo.core.entity.Entity;
import org.cryo.core.entity.World;
import org.cryo.core.graphics.EClearColorMode;
import org.cryo.core.graphics.EClearMode;
import org.cryo.core.graphics.IDevice;
import org.cryo.core.graphics.IFrameRenderer;
import org.cryo.core.math.Color4f;
import org.cryo.core.window.JavaWindow;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;

public class TestCanvas extends AWTGLCanvas {

    private final Window window = new Window();

    private final Viewport viewport = new Viewport();

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

    public JavaWindow getWindow() {
        return this.window;
    }

    @Override
    public void initGL() {
        Main.initializeEngine(new String[0], this);

        World world = new World();

        Entity cameraEntity = new Entity();
        CameraState cameraState = new CameraState();
        cameraState.setClearMode(EClearMode.DEPTH);
        cameraState.setClearColor(new Color4f(0.5f, 0.0f, 0.0f, 1.0f));
        cameraState.setClearColorMode(EClearColorMode.PLAIN_COLOR);

        cameraEntity.attachState(cameraState);
        world.attach(cameraEntity);
        world.setMainCamera(cameraState);


        viewport.setWindow(this.window);
        viewport.setDevice(ObjectRegistry.get(IDevice.class));
        viewport.setWorld(world);
        viewport.setFrameRenderer(ObjectRegistry.get(IFrameRenderer.class));



    }

    @Override
    public void paintGL() {

        viewport.processFrame();

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


    private class Window extends JavaWindow {


        @Override
        public void setTitle(String title) {
            //
        }

        @Override
        public String getTitle() {
            return "";
        }

        @Override
        public void setPosition(short x, short y) {
            //
        }

        @Override
        public short getPositionX() {
            return 0;
        }

        @Override
        public short getPositionY() {
            return 0;
        }

        @Override
        public void setResolution(int width, int height) {
            //
        }

        @Override
        public int getWidth() {
            return (short) TestCanvas.this.getWidth();
        }

        @Override
        public int getHeight() {
            return (short) TestCanvas.this.getHeight();
        }

        @Override
        public void show() {

        }

        @Override
        public void hide() {

        }

        @Override
        public void present() {
            swapBuffers();
        }

        @Override
        public void processUpdates() {
            // hmm... don't know yet
        }
    }
}
