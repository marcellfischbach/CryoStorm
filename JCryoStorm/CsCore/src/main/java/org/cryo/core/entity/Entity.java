package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass(EntityNative.CS_CLASS_NAME)
public class Entity extends CsObject {

    public Entity() {
        super();
    }

    public Entity(long ref) {
        super(ref);
    }


    public void setName (String name) {
        EntityNative.nSetName(getRef(), name);
    }

    //##BEGIN-csMOC # Don't remove

    private static native void nSetName(long ref /* this ptr */,
                                        String name /* name (const std::string&) */
                                       );

    private static native String nGetName(long ref /* this ptr */
                                         );

    private static native boolean nAttach(long ref /* this ptr */,
                                          long entityState /* entityState (cs::csEntityState*) */
                                         );

    private static native boolean nDetach(long ref /* this ptr */,
                                          long entityState /* entityState (cs::csEntityState*) */
                                         );

    //##END-csMOC # Don't remove
}



