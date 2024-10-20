package org.cryo.core.entity;

public class World
{
//##BEGIN-csMOC # Don't remove
  private static native void nSetScene(long ref /* this ptr */,
                                       long arg0 /* ptr cs::iGfxScene */
                                       );

  private static native Object nGetScene(long ref /* this ptr */
                                         );

  private static native void nSetPhysicsWorld(long ref /* this ptr */,
                                              long arg0 /* ptr cs::iPhysicsWorld */
                                              );

  private static native Object nGetPhysicsWorld(long ref /* this ptr */
                                                );

  private static native void nSetMainCamera(long ref /* this ptr */,
                                            long arg0 /* ptr cs::csCameraState */
                                            );

  private static native Object nGetMainCamera(long ref /* this ptr */
                                              );

  private static native boolean nAttach(long ref /* this ptr */,
                                        long arg0 /* ptr cs::csEntity */
                                        );

  private static native boolean nDetach(long ref /* this ptr */,
                                        long arg0 /* ptr cs::csEntity */
                                        );

  private static native boolean nAttachUpdateState(long ref /* this ptr */,
                                                   long arg0 /* ptr cs::csEntityState */
                                                   );

  private static native boolean nDetachUpdateState(long ref /* this ptr */,
                                                   long arg0 /* ptr cs::csEntityState */
                                                   );

  private static native void nUpdate(long ref /* this ptr */,
                                     float arg0 /* float */
                                     );


//##END-csMOC # Don't remove
}





