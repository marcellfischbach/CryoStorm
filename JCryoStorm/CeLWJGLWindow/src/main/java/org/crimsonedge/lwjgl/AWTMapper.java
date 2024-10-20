package org.crimsonedge.lwjgl;

import org.crimsonedge.core.input.EKey;
import org.crimsonedge.core.input.EMouseButton;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

public class AWTMapper {
    
    public static EKey map (KeyEvent evt) {
        int keyCode = evt.getKeyCode();
        int location = evt.getKeyLocation();
        switch (keyCode) {
            case KeyEvent.VK_ESCAPE: return EKey.K_ESCAPE;
            case KeyEvent.VK_A: return EKey.K_A;
            case KeyEvent.VK_B: return EKey.K_B;
            case KeyEvent.VK_C: return EKey.K_C;
            case KeyEvent.VK_D: return EKey.K_D;
            case KeyEvent.VK_E: return EKey.K_E;
            case KeyEvent.VK_F: return EKey.K_F;
            case KeyEvent.VK_G: return EKey.K_G;
            case KeyEvent.VK_H: return EKey.K_H;
            case KeyEvent.VK_I: return EKey.K_I;
            case KeyEvent.VK_J: return EKey.K_J;
            case KeyEvent.VK_K: return EKey.K_K;
            case KeyEvent.VK_L: return EKey.K_L;
            case KeyEvent.VK_M: return EKey.K_M;
            case KeyEvent.VK_N: return EKey.K_N;
            case KeyEvent.VK_O: return EKey.K_O;
            case KeyEvent.VK_P: return EKey.K_P;
            case KeyEvent.VK_Q: return EKey.K_Q;
            case KeyEvent.VK_R: return EKey.K_R;
            case KeyEvent.VK_S: return EKey.K_S;
            case KeyEvent.VK_T: return EKey.K_T;
            case KeyEvent.VK_U: return EKey.K_U;
            case KeyEvent.VK_V: return EKey.K_V;
            case KeyEvent.VK_W: return EKey.K_W;
            case KeyEvent.VK_X: return EKey.K_X;
            case KeyEvent.VK_Y: return EKey.K_Y;
            case KeyEvent.VK_Z: return EKey.K_Z;
            
            case KeyEvent.VK_0: return EKey.K_0;
            case KeyEvent.VK_1: return EKey.K_1;
            case KeyEvent.VK_2: return EKey.K_2;
            case KeyEvent.VK_3: return EKey.K_3;
            case KeyEvent.VK_4: return EKey.K_4;
            case KeyEvent.VK_5: return EKey.K_5;
            case KeyEvent.VK_6: return EKey.K_6;
            case KeyEvent.VK_7: return EKey.K_7;
            case KeyEvent.VK_8: return EKey.K_8;
            case KeyEvent.VK_9: return EKey.K_9;


            case KeyEvent.VK_SHIFT: return location == KeyEvent.KEY_LOCATION_LEFT ? EKey.K_LEFT_SHIFT : EKey.K_RIGHT_SHIFT;
            case KeyEvent.VK_ALT:  return location == KeyEvent.KEY_LOCATION_LEFT ? EKey.K_LEFT_ALT : EKey.K_RIGHT_ALT;
            case KeyEvent.VK_CONTROL:  return location == KeyEvent.KEY_LOCATION_LEFT ? EKey.K_LEFT_CTRL : EKey.K_RIGHT_CTRL;
            case KeyEvent.VK_WINDOWS:  return location == KeyEvent.KEY_LOCATION_LEFT ? EKey.K_LEFT_WIN : EKey.K_RIGHT_WIN;

            case KeyEvent.VK_SPACE: return EKey.K_SPACE;

            case KeyEvent.VK_LEFT: return EKey.K_LEFT;
            case KeyEvent.VK_RIGHT: return EKey.K_RIGHT;
            case KeyEvent.VK_UP: return EKey.K_UP;
            case KeyEvent.VK_DOWN: return EKey.K_DOWN;
        }
        return null;
    }

    public static EMouseButton map (MouseEvent event) {
        switch (event.getButton()) {
            case 1: return EMouseButton.LEFT;
            case 2: return EMouseButton.MIDDLE;
            case 3: return EMouseButton.RIGHT;
            case 4: return EMouseButton.ALT_3;
            case 5: return EMouseButton.ALT_4;
            case 6: return EMouseButton.ALT_5;
            case 7: return EMouseButton.ALT_6;
            case 8: return EMouseButton.ALT_7;
        }
        return null;
    }

}
