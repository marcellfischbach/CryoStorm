package org.crimsonedge.launcher;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class LwjglFrame extends JPanel {

    private Timer timer = new Timer(500, this::handleResize);

    public LwjglFrame(String[] args) {
        setLayout(new BorderLayout());
        add(new LwjglWindowCanvas(args), new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));



        addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                super.componentResized(e);
                timer.restart();
            }
        });
    }

    private void handleResize (ActionEvent e) {
        System.out.println("Resized: " + getWidth() + " x " + getHeight());

    }
}
