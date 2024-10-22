package org.cryo.core.graphics;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import java.util.UUID;

@CsClass(IFrameRendererNative.CS_CLASS_NAME)
public class IFrameRenderer extends CsObject {

    public final String name = UUID.randomUUID().toString();

    public IFrameRenderer() {
    }

    public IFrameRenderer(long ref) {
        super(ref);
    }
}
