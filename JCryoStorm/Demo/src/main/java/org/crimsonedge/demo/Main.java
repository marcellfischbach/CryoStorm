package org.crimsonedge.demo;

import org.crimsonedge.lwjgl.CeLWJGL;
import org.crimsonedge.lwjgl.LwjglWindowCanvas;

import javax.swing.*;
import java.awt.*;

public class Main {

    public static void main(String[] args) {
        CeLWJGL.initialize();

        SwingUtilities.invokeLater(() -> {

            JFrame frame = new JFrame();
            frame.setSize(1024, 768);
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.setLocation(100, 100);
            frame.setLayout(new BorderLayout());
            frame.add(new LwjglWindowCanvas(args, new LauncherGame()), BorderLayout.CENTER);
            frame.setVisible(true);

        });
    }
}
