package org.crimsonedge.demo.scene;

import org.crimsonedge.core.entity.csJavaState;
import org.crimsonedge.core.entity.csSpatialState;
import org.crimsonedge.core.entity.csWorld;

public class MyState extends csJavaState {

    private csSpatialState root;

    public MyState() {
    }

    public MyState(long ref) {
        super(ref);
    }

    @Override
    public void onAttachedToWorld(csWorld world) {
        root = getRoot();
    }

    @Override
    public void onDetachedFromWorld(csWorld world) {
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
