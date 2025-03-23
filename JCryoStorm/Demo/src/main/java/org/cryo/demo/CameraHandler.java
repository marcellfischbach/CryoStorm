package org.cryo.demo;

import org.cryo.core.entity.CsJavaState;
import org.cryo.core.entity.CsTransform;
import org.cryo.core.input.EMouseButton;
import org.cryo.core.input.CsInput;
import org.cryo.core.math.CsVector3f;

public class CameraHandler extends CsJavaState
{

    private final CsVector3f position = new CsVector3f(5.0f, 5.0f, 5.0f);
    private final CsVector3f target = new CsVector3f(0.0f, 0.0f, 0.0f);
    private float distance = 1.0f;
    private float time = 0.0f;

    public CameraHandler() {
    }

    @Override
    public void update(float deltaTime) {
        CsTransform tr = getRoot().getTransform();

        time += deltaTime;
        time = time % (float) (Math.PI * 2.0);
        CsVector3f pos = new CsVector3f(position);
        pos.y += ((float) Math.sin(time) * 0.5f + 0.5f) * distance;
        tr.setTranslation(pos)
                .lookAt(target, CsVector3f.Y)
                .finish();

        if (CsInput.getMouse().isButtonPressed(EMouseButton.RIGHT))
        {
            System.out.println("Pressed right");
        }
        if (CsInput.getMouse().isButtonReleased(EMouseButton.RIGHT))
        {
            System.out.println("Released right");
        }

//        System.out.println("MousePosition: " + Input.getMousePosition() + "  Delta: " + Input.getMouseDelta());
    }
}
