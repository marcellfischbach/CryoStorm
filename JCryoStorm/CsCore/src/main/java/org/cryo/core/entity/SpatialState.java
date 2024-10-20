package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.entity.EntityState;

@CsClass("cs::csSpatialState")
public class SpatialState extends EntityState {

    public SpatialState() {
        super();
    }

    public SpatialState(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

    private static native void nSetStatic(long ref /* this ptr */,
                                          boolean _static /* _static (bool) */
                                         );

    private static native boolean nIsStatic(long ref /* this ptr */
                                           );

    private static native boolean nAttach(long ref /* this ptr */,
                                          long child /* child (cs::csSpatialState*) */
                                         );

    private static native boolean nDetachSelf(long ref /* this ptr */
                                             );

    private static native boolean nDetach(long ref /* this ptr */,
                                          long child /* child (cs::csSpatialState*) */
                                         );

    private static native Object nGetParent(long ref /* this ptr */
                                           );

    private static native long nGetNumberOfChildren(long ref /* this ptr */
                                                   );

    private static native Object nGetChild(long ref /* this ptr */,
                                           long idx /* idx (size_t) */
                                          );

    //##END-csMOC # Don't remove
}



