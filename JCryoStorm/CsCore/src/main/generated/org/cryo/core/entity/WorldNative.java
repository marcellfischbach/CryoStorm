package org.cryo.core.entity;


/**
 * cs::csWorld
 */
public abstract class WorldNative {
    
    public static final String CS_CLASS_NAME = "cs::csWorld";
    
    private WorldNative() {
    }



    /**
     * void cs::csWorld::SetScene(cs::iGfxScene* scene)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param scene scene (cs::iGfxScene*)
     */
    public static native void nSetScene(long ref, long scene);

    /**
     * cs::iGfxScene* cs::csWorld::GetScene()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetScene(long ref);

    /**
     * void cs::csWorld::SetPhysicsWorld(cs::iPhysicsWorld* world)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param world world (cs::iPhysicsWorld*)
     */
    public static native void nSetPhysicsWorld(long ref, long world);

    /**
     * cs::iPhysicsWorld* cs::csWorld::GetPhysicsWorld()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetPhysicsWorld(long ref);

    /**
     * void cs::csWorld::SetMainCamera(cs::csCameraState* mainCamera)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param mainCamera mainCamera (cs::csCameraState*)
     */
    public static native void nSetMainCamera(long ref, long mainCamera);

    /**
     * cs::csCameraState* cs::csWorld::GetMainCamera()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native Object nGetMainCamera(long ref);

    /**
     * bool cs::csWorld::Attach(cs::csEntity* entity)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param entity entity (cs::csEntity*)
     */
    public static native boolean nAttach(long ref, long entity);

    /**
     * bool cs::csWorld::Detach(cs::csEntity* entity)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param entity entity (cs::csEntity*)
     */
    public static native boolean nDetach(long ref, long entity);

    /**
     * bool cs::csWorld::AttachUpdateState(cs::csEntityState* updateState)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param updateState updateState (cs::csEntityState*)
     */
    public static native boolean nAttachUpdateState(long ref, long updateState);

    /**
     * bool cs::csWorld::DetachUpdateState(cs::csEntityState* updateState)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param updateState updateState (cs::csEntityState*)
     */
    public static native boolean nDetachUpdateState(long ref, long updateState);

    /**
     * void cs::csWorld::Update(float tpf)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param tpf tpf (float)
     */
    public static native void nUpdate(long ref, float tpf);

    //##END-csMOC # Don't remove
}


