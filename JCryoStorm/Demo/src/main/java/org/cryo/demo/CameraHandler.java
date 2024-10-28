package org.cryo.demo;

import org.cryo.core.entity.JavaState;
import org.cryo.core.entity.Transform;
import org.cryo.core.math.Matrix4f;
import org.cryo.core.math.Vector3f;

public class CameraHandler extends JavaState {

    private final Vector3f position = new Vector3f(5.0f, 5.0f, 5.0f);
    private final Vector3f target = new Vector3f(0.0f, 0.0f, 0.0f);
    private float distance = 1.0f;
    private float time = 0.0f;

    public CameraHandler() {
    }

    @Override
    public void update(float deltaTime) {
        Transform tr = getRoot().getTransform();

        time += deltaTime;
        time = time % (float) (Math.PI * 2.0);
        Vector3f pos = new Vector3f(position);
        pos.y += ((float) Math.sin(time) * 0.5f + 0.5f) * distance;
        tr.setTranslation(pos)
                .lookAt(target, Vector3f.Y)
                .finish();


    }
}
