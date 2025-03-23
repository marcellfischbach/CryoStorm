package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.ITerrainMesh;

import static org.cryo.core.entity.CsTerrainMeshStateNative.*;

@CsClass(CS_CLASS_NAME)
public class CsTerrainMeshState extends CsSpatialState
{

    public CsTerrainMeshState() {
    }

    public CsTerrainMeshState(long ref) {
        super(ref);
    }

    public void setTerrainMesh(ITerrainMesh terrainMesh) {
        nSetTerrainMesh(getRef(), terrainMesh != null ? terrainMesh.getRef() : 0);
    }

    public ITerrainMesh getTerrainMesh() {
        return (ITerrainMesh) nGetTerrainMesh(getRef());
    }

    public void setLayerMask(CsTerrainLayerMask layerMask) {
        nSetLayerMask(getRef(), layerMask != null ? layerMask.getRef() : 0);
    }

    public void addLayer(CsTerrainLayer layer) {
        nAddLayer(getRef(), layer != null ? layer.getRef() : 0);
    }

    public void setReceiveShadow(boolean receiveShadow) {
        nSetReceiveShadow(getRef(), receiveShadow);
    }

    public boolean isReceiveShadow() {
        return nIsReceiveShadow(getRef());
    }

    public void setCastShadow(boolean castShadow) {
        nSetCastShadow(getRef(), castShadow);
    }

    public boolean isCastShadow() {
        return nIsCastShadow(getRef());
    }

    public void setReference(CsSpatialState reference) {
        nSetReference(getRef(), reference != null ? reference.getRef() : 0);
    }

    public CsSpatialState getReference() {
        return (CsSpatialState) nGetReference(getRef());
    }

}
