package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass("cs::csWorld")
public class World extends CsObject {

    public World() {
        super();
    }

    public World(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

    private static native void nSetScene(long ref /* this ptr */,
                                         long scene /* scene (cs::iGfxScene*) */
                                        );

    private static native Object nGetScene(long ref /* this ptr */
                                          );

    private static native void nSetPhysicsWorld(long ref /* this ptr */,
                                                long world /* world (cs::iPhysicsWorld*) */
                                               );

    private static native Object nGetPhysicsWorld(long ref /* this ptr */
                                                 );

    private static native void nSetMainCamera(long ref /* this ptr */,
                                              long mainCamera /* mainCamera (cs::csCameraState*) */
                                             );

    private static native Object nGetMainCamera(long ref /* this ptr */
                                               );

    private static native boolean nAttach(long ref /* this ptr */,
                                          long entity /* entity (cs::csEntity*) */
                                         );

    private static native boolean nDetach(long ref /* this ptr */,
                                          long entity /* entity (cs::csEntity*) */
                                         );

    private static native boolean nAttachUpdateState(long ref /* this ptr */,
                                                     long updateState /* updateState (cs::csEntityState*) */
                                                    );

    private static native boolean nDetachUpdateState(long ref /* this ptr */,
                                                     long updateState /* updateState (cs::csEntityState*) */
                                                    );

    private static native void nUpdate(long ref /* this ptr */,
                                       float tpf /* tpf (float) */
                                      );

    //##END-csMOC # Don't remove
}



