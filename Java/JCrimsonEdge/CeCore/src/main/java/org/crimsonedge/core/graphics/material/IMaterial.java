package org.crimsonedge.core.graphics.material;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.ICoreObject;
import org.crimsonedge.core.graphics.EFillMode;
import org.crimsonedge.core.graphics.ERenderQueue;
import org.crimsonedge.core.graphics.EShadingMode;
import org.crimsonedge.core.graphics.ITexture;
import org.crimsonedge.core.math.*;

@CeClass("cryo::iMaterial")
public interface IMaterial extends ICoreObject {

      EFillMode getFillMode()  ;
      ERenderQueue getRenderQueue()  ;
      EShadingMode getShadingMode()  ;

//     bool Bind(iDevice * device, eRenderPass pass) ;

     int indexOf(String  attributeName) ;
     default void debug (int idx) {}

     void set(int idx, float value) ;
     void set(int idx, Vector2f v) ;
     void set(int idx, Vector3f v) ;
     void set(int idx, Vector4f v) ;
     void set(int idx, Color4f v) ;
     void set(int idx, int value) ;
     void set(int idx, Matrix3f m) ;
     void set(int idx, Matrix4f m) ;
     void set(int idx, ITexture texture) ;
}
