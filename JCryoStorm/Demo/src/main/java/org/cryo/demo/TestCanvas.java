package org.cryo.demo;

import org.cryo.core.CsObjectRegistry;
import org.cryo.core.Viewport;
import org.cryo.core.entity.CsCameraState;
import org.cryo.core.entity.CsEntity;
import org.cryo.core.entity.CsWorld;
import org.cryo.core.graphics.*;
import org.cryo.core.graphics.material.IMaterial;
import org.cryo.core.math.CsColor4f;
import org.cryo.core.resource.CsAssetManager;
import org.cryo.core.window.CsWindowJava;
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

    public CsWindowJava getWindow() {
        return this.window;
    }

    @Override
    public void initGL() {
        Main.initializeEngine(new String[0]);

        CsWorld world = new CsWorld();

        CsEntity cameraEntity = new CsEntity();
        CsCameraState cameraState = new CsCameraState();
        cameraState.setClearMode(EClearMode.DEPTH_COLOR);
        cameraState.setClearColor(new CsColor4f(0.5f, 0.0f, 0.0f, 1.0f));
        cameraState.setClearColorMode(EClearColorMode.PLAIN_COLOR);

        cameraEntity.attachState(cameraState);
        world.attach(cameraEntity);
        world.setMainCamera(cameraState);


        viewport.setWindow(this.window);
        viewport.setDevice(CsObjectRegistry.get(IDevice.class));
        viewport.setWorld(world);
        viewport.setRenderPipeline(ERenderPipeline.DEFERRED);


        IMaterial defaultMat = CsAssetManager.instance().get(IMaterial.class, "materials/Default.mat");
        IMaterial defaultMatBlue = CsAssetManager.instance().get(IMaterial.class, "materials/DefaultBlue.matinstance");

        System.out.println();
    }

    @Override
    public void paintGL() {

        viewport.processFrame(null);

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


    private class Window extends CsWindowJava
    {


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
