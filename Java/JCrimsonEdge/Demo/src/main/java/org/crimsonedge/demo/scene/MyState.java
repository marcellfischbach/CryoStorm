package org.crimsonedge.demo.scene;

import org.crimsonedge.core.entity.JavaState;
import org.crimsonedge.core.entity.SpatialState;
import org.crimsonedge.core.entity.World;

public class MyState extends JavaState {

    private SpatialState root;

    public MyState() {
    }

    public MyState(long ref) {
        super(ref);
    }

    @Override
    public void onAttachedToWorld(World world) {
        root = getRoot();
    }

    @Override
    public void onDetachedFromWorld(World world) {
    }


    private float rot = 0.0f;

    @Override
    public void update(float tpf) {

        rot += tpf;

        if (root == null) {
            return;
        }


        root.getTransform()
            .setTranslation((float) Math.cos(rot) * 5.0f, 5.0f, (float) Math.sin(rot) * 5.0f)
            .finish();
    }
}
