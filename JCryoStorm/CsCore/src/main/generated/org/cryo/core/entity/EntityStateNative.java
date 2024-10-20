package org.cryo.core.entity;


/**
 * cs::csEntityState
 */
public abstract class EntityStateNative {
    
    public static final String CS_CLASS_NAME = "cs::csEntityState";
    
    private EntityStateNative() {
    }



    /**
     * void cs::csEntityState::SetName(const std::string& name)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param name name (const std::string&)
     */
    public static native void nSetName(long ref, String name);

    /**
     * const std::string& cs::csEntityState::GetName()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native String nGetName(long ref);

    /**
     * void cs::csEntityState::SetEntity(cs::csEntity* entity)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param entity entity (cs::csEntity*)
     */
    public static native void nSetEntity(long ref, long entity);

    /**
     * cs::csEntity* cs::csEntityState::GetEntity()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetEntity(long ref);

    /**
     * cs::csSpatialState* cs::csEntityState::GetRoot()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetRoot(long ref);

    /**
     * cs::csWorld* cs::csEntityState::GetWorld()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetWorld(long ref);

    /**
     * void cs::csEntityState::AttachToWorld(cs::csWorld* world)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param world world (cs::csWorld*)
     */
    public static native void nAttachToWorld(long ref, long world);

    /**
     * void cs::csEntityState::DetachFromWorld(cs::csWorld* world)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param world world (cs::csWorld*)
     */
    public static native void nDetachFromWorld(long ref, long world);

    /**
     * void cs::csEntityState::SetNeedUpdate(bool needUpdate)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param needUpdate needUpdate (bool)
     */
    public static native void nSetNeedUpdate(long ref, boolean needUpdate);

    /**
     * bool cs::csEntityState::IsNeedUpdate()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native boolean nIsNeedUpdate(long ref);

    //##END-csMOC # Don't remove
}


