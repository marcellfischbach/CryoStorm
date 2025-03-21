package org.cryo.lwjgl.window;

import org.cryo.core.ObjectRegistry;
import org.cryo.core.Viewport;
import org.cryo.core.graphics.ERenderPipeline;
import org.cryo.core.graphics.IDevice;
import org.cryo.core.graphics.IFrameRenderer;
import org.cryo.core.window.IWindow;
import org.cryo.lwjgl.window.internal.IntGlCanvas;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;
import java.awt.*;

public class AwtGlCanvas extends AWTGLCanvas
{

    public String name;

    private final IWindow window = new WindowLwjgl(this);

    private Viewport viewport;


    public interface InitEngineCallback {
        void initEngine();
    }

    public interface CanvasInitializedCallback {
        void canvasInitialized(AwtGlCanvas canvas);
    }

    public interface AfterRenderCallback {
        void afterRender(AwtGlCanvas canvas);
    }

    private final InitEngineCallback initEngineCallback;
    private final CanvasInitializedCallback canvasInitializedCallback;
    private AfterRenderCallback afterRenderCallback = null;


    public AwtGlCanvas(IntGlCanvas sharedCanvas,
                       InitEngineCallback initEngineCallback,
                       CanvasInitializedCallback canvasInitializedCallback) {
        super(glData(null)); //sharedCanvas));


        this.initEngineCallback = initEngineCallback;
        this.canvasInitializedCallback = canvasInitializedCallback;

        initialize();
    }


    public void setAfterRenderCallback(AfterRenderCallback afterRenderCallback) {
        this.afterRenderCallback = afterRenderCallback;
    }

    private boolean renderLoopActive = false;

    public void startRenderLoop() {
        renderLoopActive = true;
        new Thread (() -> {
            while (renderLoopActive) {
                SwingUtilities.invokeLater(this::render);
                try {
                    Thread.sleep(1);
                }
                catch (Exception e) {
                    //
                }
            }
        }).start();

//        Runnable loop = new Runnable() {
//            @Override
//            public void run() {
//                if (renderLoopActive) {
//                    SwingUtilities.invokeLater(this);
//                }
//            }
//        };
//        SwingUtilities.invokeLater(loop);
    }


    public void stopRenderLoop() {
        this.renderLoopActive = false;
    }

    public IWindow getWindow() {
        return this.window;
    }

    public Viewport getViewport() {
        return this.viewport;
    }

    @Override
    public void initGL() {

        this.initEngineCallback.initEngine();



        this.viewport = new Viewport();
        this.viewport.setWindow(this.window);
        this.viewport.setDevice(ObjectRegistry.get(IDevice.class));
        this.viewport.setRenderPipeline(ERenderPipeline.DEFERRED);

        this.canvasInitializedCallback.canvasInitialized(this);


    }

    public void initialize() {
        SwingUtilities.invokeLater(() -> {
            beforeRender();
            try {
                initGL();
                initCalled = true;
            }
            finally {
                afterRender();
            }
        });
    }

    protected void afterRender() {
//        platformCanvas.makeCurrent(0L);
        try {
            platformCanvas.unlock(); // <- MUST unlock on Linux
        } catch (AWTException e) {
            throw new RuntimeException("Failed to unlock Canvas", e);
        }
    }

    @Override
    public void paintGL() {
        viewport.processFrame(null);

        if (this.afterRenderCallback != null) {
            this.afterRenderCallback.afterRender(this);
        }
        swapBuffers();
    }


    private static GLData glData(AWTGLCanvas sharedCanvas) {
        GLData data = new GLData();
        data.majorVersion = 4;
        data.minorVersion = 4;
        data.api = GLData.API.GL;
        data.profile = GLData.Profile.COMPATIBILITY;
        data.swapInterval = 0;
        data.shareContext = null; //sharedCanvas;
        data.robustness = true;
        data.forwardCompatible = false;
        System.out.println("Shared context: " + sharedCanvas);
        return data;
    }


}

