package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.entity.SpatialState;

@CsClass(LightStateNative.CS_CLASS_NAME)
public class LightState extends SpatialState {

    public LightState(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

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

    private static native void nSetRange(long ref /* this ptr */,
                                         float range /* range (float) */
                                        );

    private static native float nGetRange(long ref /* this ptr */
                                         );

    //##END-csMOC # Don't remove
}



