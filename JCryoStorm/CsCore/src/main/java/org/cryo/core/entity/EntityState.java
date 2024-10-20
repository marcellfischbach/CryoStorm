package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass("cs::csEntityState")
public class EntityState extends CsObject {

    public EntityState() {
        super();
    }

    public EntityState(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

    private static native void nSetName(long ref /* this ptr */,
                                        String name /* name (const std::string&) */
                                       );

    private static native String nGetName(long ref /* this ptr */
                                         );

    private static native void nSetEntity(long ref /* this ptr */,
                                          long entity /* entity (cs::csEntity*) */
                                         );

    private static native Object nGetEntity(long ref /* this ptr */
                                           );

    private static native Object nGetRoot(long ref /* this ptr */
                                         );

    private static native Object nGetWorld(long ref /* this ptr */
                                          );

    private static native void nAttachToWorld(long ref /* this ptr */,
                                              long world /* world (cs::csWorld*) */
                                             );

    private static native void nDetachFromWorld(long ref /* this ptr */,
                                                long world /* world (cs::csWorld*) */
                                               );

    private static native void nSetNeedUpdate(long ref /* this ptr */,
                                              boolean needUpdate /* needUpdate (bool) */
                                             );

    private static native boolean nIsNeedUpdate(long ref /* this ptr */
                                               );

    //##END-csMOC # Don't remove
}



