package org.cryo.demo;

import org.cryo.core.entity.CsJavaState;
import org.cryo.core.entity.CsTransform;
import org.cryo.core.entity.CsWorld;
import org.cryo.core.input.*;
import org.cryo.core.math.CsQuaternion;
import org.cryo.core.math.CsVector2f;
import org.cryo.core.math.CsVector3f;

public class CameraHandlerComponent extends CsJavaState
{

    private float speed = 4.0f;

    private float rotSpeed = 0.0025f;

    private float rotY = 0.46f;
    private float rotX = -0.58f;

    public CameraHandlerComponent() {

    }

    @Override
    public void onAttachedToWorld(CsWorld world)
    {
        super.onAttachedToWorld(world);
        getRoot().getTransform().setTranslation(new CsVector3f(0.0f, 10.0f, 0.0f)).finish();

    }

    @Override
    public void update(float deltaTime) {

        CsTransform tr = getRoot().getTransform();

        CsVector3f dir = new CsVector3f();

        IMouse iMouse = CsInput.getMouse();
        IKeyboard iKeyboard = CsInput.getKeyboard();

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


            CsVector2f mouse = new CsVector2f(iMouse.getDeltaX(), iMouse.getDeltaY());
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


        CsQuaternion qRotY = CsQuaternion.fromAxisAngle(0.0f, 1.0f, 0.0f, rotY);
        CsQuaternion qRotX = CsQuaternion.fromAxisAngle(1.0f, 0.0f, 0.0f, rotX);
        CsQuaternion qRot  = new CsQuaternion(qRotX).mul(qRotY);


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

        tr.setTranslation(tr.getTranslation(new CsVector3f()).add(dir))
          .setRotation(qRot)
          .finish();
    }
}
