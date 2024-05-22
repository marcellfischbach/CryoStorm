package org.crimsonedge.core.graphics.shading;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.graphics.EShaderAttribute;

@CeClass("ce::iShader")
public class IShader extends CoreObject {

    public IShader(long ref) {
        super(ref);
    }

    private static native int nRegisterAttribute (long ref, String attributeName);
    private static native int nGetAttributeId(long ref, String attributeName);
    private static native IShaderAttribute nGetShaderAttributeId (long ref, int id);
    private static native IShaderAttribute nGetShaderAttributeName (long ref, String attributeName);

    public int registerAttribute (String attributeName){
        return nRegisterAttribute(getRef(), attributeName);
    }

    public int getAttributeId (String attributeName){
        return nGetAttributeId(getRef(), attributeName);
    }

    public IShaderAttribute getShaderAttribute (int id){
        return nGetShaderAttributeId(getRef(), id);
    }

    public IShaderAttribute getShaderAttribute (String attributeName){
        return nGetShaderAttributeName(getRef(), attributeName);
    }

    public IShaderAttribute getShaderAttribute (EShaderAttribute attribute){
        return nGetShaderAttributeId(getRef(), attribute.ordinal());
    }
}
