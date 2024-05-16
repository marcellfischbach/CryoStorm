package org.crimsonedge.launcher;

import org.crimsonedge.core.input.EKey;
import org.crimsonedge.core.input.IKeyboard;

public class LwjglKeyboard implements IKeyboard {
    
    private final long ref;
    
    private boolean[] keys = new boolean[EKey.values().length]; 
    private boolean[] prevKeys = new boolean[EKey.values().length];


    public static final class Event {
        int key;
        boolean down;
    }

    private Event[] events = new Event[1000];
    private int numEvents = 0;

    public LwjglKeyboard(long ref) {
        this.ref = ref;
        for (int i = 0; i < events.length; i++) {
            events[i] = new Event();
        }
    }

    public void pushEvent (int key, boolean down) {
        Event event = events[numEvents++];
        event.key = key;
        event.down = down;
    }

    public boolean isKeyDown(int key) {
        return keys[key];
    }

    public boolean isKeyUp(int key) {
        return !keys[key];
    }

    public boolean isKeyPressed(int key) {
        return keys[key] && !prevKeys[key];
    }

    public boolean isKeyReleased(int key) {
        return !keys[key] && prevKeys[key];
    }

    @Override
    public boolean isKeyDown(EKey key) {
        return isKeyDown(key.ordinal());
    }

    @Override
    public boolean isKeyUp(EKey key) {
        return isKeyUp(key.ordinal());
    }

    @Override
    public boolean isKeyPressed(EKey key) {
        return isKeyPressed(key.ordinal());
    }

    @Override
    public boolean isKeyReleased(EKey key) {
        return isKeyReleased(key.ordinal());
    }
    public void update() {
        System.arraycopy(this.keys, 0, this.prevKeys, 0, this.keys.length);
        for (int i=0; i<numEvents; i++) {
            Event evt = this.events[i];
            this.keys[evt.key] = evt.down;
        }
        numEvents = 0;
    }
}
