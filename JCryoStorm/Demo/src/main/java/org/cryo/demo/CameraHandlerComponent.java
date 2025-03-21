package org.cryo.demo;

import org.cryo.core.entity.JavaState;
import org.cryo.core.entity.Transform;
import org.cryo.core.entity.World;
import org.cryo.core.input.*;
import org.cryo.core.math.Quaternion;
import org.cryo.core.math.Vector2f;
import org.cryo.core.math.Vector3f;

public class CameraHandlerComponent extends JavaState {

    private float speed = 4.0f;

    private float rotSpeed = 0.0025f;

    private float rotY = 0.46f;
    private float rotX = -0.58f;

    public CameraHandlerComponent() {

    }

    @Override
    public void onAttachedToWorld(World world)
    {
        super.onAttachedToWorld(world);
        getRoot().getTransform().setTranslation(new Vector3f(0.0f, 10.0f, 0.0f)).finish();

    }

    @Override
    public void update(float deltaTime) {

        Transform tr = getRoot().getTransform();

        Vector3f dir = new Vector3f();

        IMouse iMouse = Input.getMouse();
        IKeyboard iKeyboard = Input.getKeyboard();

        if (iMouse.isButtonDown(EMouseButton.RIGHT))
        {
            if (iKeyboard.isKeyDown(EKey.KEY_W))
            {
                dir.add(tr.getForward().mul(speed * deltaTime));
            }
            if (iKeyboard.isKeyDown(EKey.KEY_S))
            {
                dir.add(tr.getBackward().mul(speed * deltaTime));
            }
            if (iKeyboard.isKeyDown(EKey.KEY_A))
            {
                dir.add(tr.getLeft().mul(speed * deltaTime));
            }
            if (iKeyboard.isKeyDown(EKey.KEY_D))
            {
                dir.add(tr.getRight().mul(speed * deltaTime));
            }

            if (iKeyboard.isKeyDown(EKey.KEY_LEFT_SHIFT) || iKeyboard.isKeyDown(EKey.KEY_RIGHT_SHIFT))
            {
                dir.mul(2.0f);
            }


            Vector2f mouse = new Vector2f(iMouse.getDeltaX(), iMouse.getDeltaY());
            this.rotX += -mouse.y * this.rotSpeed;
            this.rotY += -mouse.x * this.rotSpeed;

            float clamp = (float) Math.PI * 0.48f;
            float pi2   = (float) Math.PI * 2.0f;
            rotX = Math.clamp(rotX, -clamp, clamp);

            while (rotY > pi2)
            {
                rotY -= pi2;
            }
            while (rotY < 0)
            {
                rotY += pi2;
            }
        }


        Quaternion qRotY = Quaternion.fromAxisAngle(0.0f, 1.0f, 0.0f, rotY);
        Quaternion qRotX = Quaternion.fromAxisAngle(1.0f, 0.0f, 0.0f, rotX);
        Quaternion qRot  = new Quaternion(qRotX).mul(qRotY);


//        if (csInput::IsKeyPressed(eKey::eK_P))
//        {
//            printf ("(%.2f %.2f %.2f) @ (%.2f %.2f)\n",
//                    tr.GetTranslation().x,
//                    tr.GetTranslation().y,
//                    tr.GetTranslation().z,
//                    m_rotX,
//                    m_rotY
//            );
//            fflush(stdout);
//        }

        tr.setTranslation(tr.getTranslation(new Vector3f()).add(dir))
          .setRotation(qRot)
          .finish();
    }
}
