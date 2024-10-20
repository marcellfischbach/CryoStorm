package org.cryo.core.entity;


/**
 * cs::csEntity
 */
public abstract class EntityNative {
    
    public static final String CS_CLASS_NAME = "cs::csEntity";
    
    private EntityNative() {
    }



    /**
     * void cs::csEntity::SetName(const std::string& name)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param name name (const std::string&)
     */
    public static native void nSetName(long ref, String name);

    /**
     * const std::string& cs::csEntity::GetName()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native String nGetName(long ref);

    /**
     * bool cs::csEntity::Attach(cs::csEntityState* entityState)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param entityState entityState (cs::csEntityState*)
     */
    public static native boolean nAttach(long ref, long entityState);

    /**
     * bool cs::csEntity::Detach(cs::csEntityState* entityState)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param entityState entityState (cs::csEntityState*)
     */
    public static native boolean nDetach(long ref, long entityState);

    //##END-csMOC # Don't remove
}


