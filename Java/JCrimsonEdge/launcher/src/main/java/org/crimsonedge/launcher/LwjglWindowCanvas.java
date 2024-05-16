package org.crimsonedge.launcher;

import org.crimsonedge.core.Engine;
import org.crimsonedge.core.input.EKey;
import org.crimsonedge.core.input.EMouseButton;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class LwjglWindowCanvas extends AWTGLCanvas  {

    private String[] args;

    private static native long nInitWindow (LwjglWindowCanvas window);

    private long wnd;

    private final LwjglWindow window;

    private int renderWidth;

    private int renderHeight;

    public LwjglWindowCanvas(String[] args) {
        super(glData());

        this.renderWidth = 1024;
        this.renderHeight = 768;

        this.window = new LwjglWindow(this);
        this.args = args;


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

        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                EKey key = AWTMapper.map(e);
                if (key != null) {
                    window.getKeyboard().pushEvent(key.ordinal(), true);
                }
            }

            @Override
            public void keyReleased(KeyEvent e) {
                EKey key = AWTMapper.map(e);
                if (key != null) {
                    window.getKeyboard().pushEvent(key.ordinal(), false);
                }
            }
        });

        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                EMouseButton button = AWTMapper.map(e);
                if (button != null) {
                    window.getMouse().pushEvent(button.ordinal(), true);
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                EMouseButton button = AWTMapper.map(e);
                if (button != null) {
                    window.getMouse().pushEvent(button.ordinal(), false);
                }
            }
        });

        addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                timer.restart();
            }
        });
        timer.restart();
    }


    private Timer timer = new Timer(200, e -> handleResized());

    private void handleResized() {
        this.renderWidth = getWidth();
        this.renderHeight = getHeight();
    }


    public int getRenderWidth() {
        return renderWidth;
    }

    public int getRenderHeight() {
        return renderHeight;
    }

    @Override
    public void initGL() {

        Engine.instance().initialize(this.args, new LauncherGame());
    }

    @Override
    public void paintGL() {



        window.getKeyboard().update();
        window.getMouse().update();


        Engine.instance().processFrame();

        swapBuffers();

        if (window.getKeyboard().isKeyDown(EKey.K_ESCAPE)) {
            System.exit(0);
        }
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
