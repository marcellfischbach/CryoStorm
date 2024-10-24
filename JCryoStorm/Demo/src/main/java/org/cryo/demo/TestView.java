package org.cryo.demo;

import org.cryo.core.Viewport;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.lang.reflect.Field;
import java.util.Arrays;

public class TestView extends JPanel {

    private final Viewport viewport;

    public static native void paint(int[] buffer);

    public TestView(Viewport viewport) {
        super();
        this.viewport = viewport;
    }

    @Override
    public void paint(Graphics g) {
        long start = System.currentTimeMillis();
        if (image == null || image.getWidth() != getWidth() || image.getHeight() != getHeight()) {
            image = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_RGB);
            System.out.println("Recreate");
        }

        Raster rasterer = image.getRaster();
        DataBufferInt intBuffer = (DataBufferInt) rasterer.getDataBuffer();
        int[] data = intBuffer.getData();
//        paint(data);

        g.drawImage(image, 0, 0, getWidth(), getHeight(), this);
        long end = System.currentTimeMillis();
        System.out.println("Time: " + (end - start));

    }

    private BufferedImage image = null;


}
