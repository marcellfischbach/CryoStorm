package org.cryo.core.entity;


/**
 * cs::csSpatialState
 */
public abstract class SpatialStateNative {
    
    public static final String CS_CLASS_NAME = "cs::csSpatialState";
    
    private SpatialStateNative() {
    }



    /**
     * void cs::csSpatialState::SetStatic(bool _static)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param _static _static (bool)
     */
    public static native void nSetStatic(long ref, boolean _static);

    /**
     * bool cs::csSpatialState::IsStatic()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native boolean nIsStatic(long ref);

    /**
     * bool cs::csSpatialState::Attach(cs::csSpatialState* child)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param child child (cs::csSpatialState*)
     */
    public static native boolean nAttach(long ref, long child);

    /**
     * bool cs::csSpatialState::DetachSelf()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native boolean nDetachSelf(long ref);

    /**
     * bool cs::csSpatialState::Detach(cs::csSpatialState* child)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param child child (cs::csSpatialState*)
     */
    public static native boolean nDetach(long ref, long child);

    /**
     * cs::csSpatialState* cs::csSpatialState::GetParent()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetParent(long ref);

    /**
     * size_t cs::csSpatialState::GetNumberOfChildren()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native long nGetNumberOfChildren(long ref);

    /**
     * cs::csSpatialState* cs::csSpatialState::GetChild(size_t idx)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param idx idx (size_t)
     */
    public static native Object nGetChild(long ref, long idx);

    //##END-csMOC # Don't remove
}


