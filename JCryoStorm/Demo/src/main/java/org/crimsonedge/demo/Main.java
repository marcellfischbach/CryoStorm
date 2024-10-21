package org.crimsonedge.demo;


import org.cryo.core.CsCoreLibrary;
import org.cryo.core.Engine;
import org.cryo.core.ModuleConfig;
import org.cryo.core.resource.VFS;
import org.cryo.core.resource.VFSConfigReader;

import javax.swing.*;
import java.awt.*;

public class Main {


    public static void main2(String[] args) {
        SwingUtilities.invokeLater(() -> {

            JFrame frame = new JFrame();
            frame.setLayout(new BorderLayout());
            frame.add(new TestCanvas(), BorderLayout.CENTER);

            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(1024, 768);
            frame.setLocation(100, 100);
            frame.setVisible(true);

        });
    }


    public static void main(String[] args) {
        CsCoreLibrary.load();

        String rootPath = "../data";
        String configFileName = "vfs_java.config";

        for (int i=0; i<args.length; i++) {
            if (args[i].equals("--data") && args.length>(i+1)) {
                rootPath = args[++i];
            }
            if (args[i].equals("--config") && args.length>(i+1)) {
                configFileName = args[++i];
            }
        }

        VFS.instance().setRootPath(rootPath);
        VFSConfigReader.read(rootPath, configFileName);

        Engine engine = Engine.get();

        ModuleConfig moduleConfig = new ModuleConfig();
        TestModule module = new TestModule();
        moduleConfig.addModule(module);
        System.out.println("Module: Ref: " + module.getRef());

        String[] nArgs = new String[] {
                "Value 0",
                "Value 1"
        };
        engine.initializeEngine(nArgs, moduleConfig);

        System.out.println("Hello World!");
//        CeLWJGL.initialize();
//
//        SwingUtilities.invokeLater(() -> {
//
//            JFrame frame = new JFrame();
//            frame.setSize(1024, 768);
//            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
//            frame.setLocation(100, 100);
//            frame.setLayout(new BorderLayout());
//            frame.add(new LwjglWindowCanvas(args, new LauncherGame()), BorderLayout.CENTER);
//            frame.setVisible(true);
//
//        });
    }
}
