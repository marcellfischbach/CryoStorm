package org.crimsonedge.core.graphics.material;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.graphics.EFillMode;
import org.crimsonedge.core.graphics.ERenderQueue;
import org.crimsonedge.core.graphics.EShadingMode;
import org.crimsonedge.core.math.Color4f;
import org.crimsonedge.core.math.Vector2f;
import org.crimsonedge.core.math.Vector3f;
import org.crimsonedge.core.math.Vector4f;

@CeClass("ce::iMaterial")
public interface IMaterial {

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
//     void Set(int idx,  Matrix3f  m) ;
//     void Set(int idx,  Matrix4f  m) ;
//     void Set(int idx, iTexture  texture) ;
}
