package org.cryo.demo;


import org.cryo.core.*;
import org.cryo.core.entity.CameraState;
import org.cryo.core.entity.Entity;
import org.cryo.core.entity.World;
import org.cryo.core.graphics.EClearColorMode;
import org.cryo.core.graphics.EClearMode;
import org.cryo.core.graphics.material.IMaterial;
import org.cryo.core.math.Color4f;
import org.cryo.core.resource.AssetManager;
import org.cryo.core.resource.VFS;
import org.cryo.core.resource.VFSConfigReader;
import org.cryo.lwjgl.window.AwtGlCanvas;

import javax.swing.*;
import java.awt.*;

public class Main {


    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {

            CsCoreLibrary.load();

            String rootPath = "d:/dev/cryostorm/data";
            String configFileName = "vfs_java.config";

            for (int i = 0; i < args.length; i++) {
                if (args[i].equals("--data") && args.length > (i + 1)) {
                    rootPath = args[++i];
                }
                if (args[i].equals("--config") && args.length > (i + 1)) {
                    configFileName = args[++i];
                }
            }

            VFS.instance().setRootPath(rootPath);
            VFSConfigReader.read(rootPath, configFileName);

            // this causes on c++ side to initialize the reflection system
            // without this call no connection between java and c++ is handled
            Engine.get();

            JFrame frame = new JFrame();
            frame.setLayout(new BorderLayout());
            AwtGlCanvas testCanvas = new AwtGlCanvas(null,
                                                     () -> initializeEngine(args),
                                                     Main::canvasInitialized);
            testCanvas.name = "Red";
            frame.add(testCanvas, BorderLayout.CENTER);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(1024, 768);
            frame.setLocation(-1100, 100);
            frame.setVisible(true);


            testCanvas.startRenderLoop();

        });
    }


    private static void canvasInitialized(AwtGlCanvas canvas) {
        World world = new World();
        canvas.getViewport().setWorld(world);

        DemoWorld.create(world);




    }



    private static boolean initialized = false;

    public static void initializeEngine(String[] args) {
        if (initialized) {
            return;
        }

        ModuleConfig moduleConfig = new ModuleConfig();
        if (!moduleConfig.loadModuleConfig()) {
            System.out.println("Failed to load module config");
            return;
        }

        Engine.get().initializeEngine(args, moduleConfig);

        initialized = true;

    }
}
