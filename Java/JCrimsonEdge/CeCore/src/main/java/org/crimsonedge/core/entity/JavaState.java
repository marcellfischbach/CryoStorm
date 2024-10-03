package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;

@CeClass("cryo::csJavaState")
public class csJavaState extends csEntityState{

    public csJavaState() {
    }

    public csJavaState(long ref) {
        super(ref);
    }

    public void onAttachedToWorld(csWorld world) {
    }

    public void onDetachedFromWorld(csWorld world) {
    }

    public void update(float tpf) {

    }

    public csSpatialState getRoot () {
        csEntity entity = getEntity();
        return entity != null ? entity.getRoot() : null;
    }
}
