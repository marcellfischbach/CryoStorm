package org.crimsonedge.launcher;


import javax.swing.*;
import java.awt.*;

public class Main {


    public static void main(String[] args) {

        Bootstrapper.bootstrap();

        SwingUtilities.invokeLater(() -> {

            JFrame frame = new JFrame();
            frame.setSize(1024, 768);
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.setLocation(100, 100);
            frame.setLayout(new GridBagLayout());
            frame.add(new LwjglWindowCanvas(args), new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
            frame.setVisible(true);

        });
    }
}
