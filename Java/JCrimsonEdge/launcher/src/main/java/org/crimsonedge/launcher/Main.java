package org.crimsonedge.launcher;


import javax.swing.*;
import java.awt.*;

public class Main {


    public static void main(String[] args) {
        JavaBinding.loadLibraries();



        SwingUtilities.invokeLater(() -> {

            JFrame frame = new JFrame();
            frame.setSize(1024, 768);
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.setLocation(100, 100);
            frame.setLayout(new BorderLayout());
            frame.add(new LwjglWindowCanvas(args), BorderLayout.CENTER);
            frame.setVisible(true);

        });
    }
}
