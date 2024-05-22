package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;

@CeClass("ce::JavaState")
public class JavaState extends EntityState{

    public JavaState() {
    }

    public JavaState(long ref) {
        super(ref);
    }

    public void onAttachedToWorld(World world) {
    }

    public void onDetachedFromWorld(World world) {
    }

    public void update(float tpf) {

    }

    public SpatialState getRoot () {
        Entity entity = getEntity();
        return entity != null ? entity.getRoot() : null;
    }
}
