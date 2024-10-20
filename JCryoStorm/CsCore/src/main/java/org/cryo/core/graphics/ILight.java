package org.cryo.core.graphics;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass("cs::iLight")
public class ILight extends CsObject {

    public ILight(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

    private static native int nGetType(long ref /* this ptr */
                                      );

    private static native void nSetChangeMode(long ref /* this ptr */,
                                              int changeMode /* changeMode (cs::eLightChangeMode) */
                                             );

    private static native int nGetChangeMode(long ref /* this ptr */
                                            );

    private static native void nSetValue(long ref /* this ptr */,
                                         int changeMode /* changeMode (cs::eLightChangeMode) */,
                                         int type /* type (cs::eLightType) */
                                        );

    private static native void nSetCastShadow(long ref /* this ptr */,
                                              boolean castShadow /* castShadow (bool) */
                                             );

    private static native boolean nIsCastShadow(long ref /* this ptr */
                                               );

    private static native void nSetShadowMapBias(long ref /* this ptr */,
                                                 float bias /* bias (float) */
                                                );

    private static native float nGetShadowMapBias(long ref /* this ptr */
                                                 );

    private static native void nSetColor(long ref /* this ptr */,
                                         float colorR /* color (const cs::csColor4f&) */,
                                         float colorG /* color (const cs::csColor4f&) */,
                                         float colorB /* color (const cs::csColor4f&) */,
                                         float colorA /* color (const cs::csColor4f&) */
                                        );

    private static native void nGetColor(long ref /* this ptr */,
                                         float[] outArg0 /* cs::csColor4f (output return value) */
                                        );

    private static native void nSetIntensity(long ref /* this ptr */,
                                             float intensity /* intensity (float) */
                                            );

    private static native float nGetIntensity(long ref /* this ptr */
                                             );

    //##END-csMOC # Don't remove
}



