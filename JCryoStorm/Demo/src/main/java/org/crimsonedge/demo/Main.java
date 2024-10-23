package org.crimsonedge.demo;


import org.cryo.core.*;
import org.cryo.core.resource.VFS;
import org.cryo.core.resource.VFSConfigReader;

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
            TestCanvas testCanvas = new TestCanvas();
            frame.add(testCanvas, BorderLayout.CENTER);

            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(1024, 768);
            frame.setLocation(100, 100);
            frame.setVisible(true);
        });
    }


    public static void initializeEngine(String[] args, TestCanvas canvas) {


        ModuleConfig moduleConfig = new ModuleConfig();
        if (!moduleConfig.loadModuleConfig()) {
            System.out.println("Failed to load module config");
            return;
        }

        Engine.get().initializeEngine(args, moduleConfig);

    }
}
