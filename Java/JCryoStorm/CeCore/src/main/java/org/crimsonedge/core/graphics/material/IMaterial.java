package org.crimsonedge.core.graphics.material;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.ICoreObject;
import org.crimsonedge.core.graphics.EFillMode;
import org.crimsonedge.core.graphics.ERenderQueue;
import org.crimsonedge.core.graphics.EShadingMode;
import org.crimsonedge.core.graphics.ITexture;
import org.crimsonedge.core.math.*;

@CeClass("cs::iMaterial")
public interface IMaterial extends ICoreObject {

      EFillMode getFillMode()  ;
      ERenderQueue getRenderQueue()  ;
      EShadingMode getShadingMode()  ;

//     bool Bind(iDevice * device, eRenderPass pass) ;

     int indexOf(String  attributeName) ;
     default void debug (int idx) {}

     void set(int idx, float value) ;
     void set(int idx, csVector2f v) ;
     void set(int idx, csVector3f v) ;
     void set(int idx, csVector4f v) ;
     void set(int idx, csColor4f v) ;
     void set(int idx, int value) ;
     void set(int idx, csMatrix3f m) ;
     void set(int idx, csMatrix4f m) ;
     void set(int idx, ITexture texture) ;
}
