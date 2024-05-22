package org.crimsonedge.core.input;

public interface IKeyboard {

    boolean isKeyDown(EKey key);
    boolean isKeyUp(EKey key);


    boolean isKeyPressed(EKey key);
    boolean isKeyReleased(EKey key);

}
