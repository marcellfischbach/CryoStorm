package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.ITerrainMesh;

import static org.cryo.core.entity.TerrainMeshStateNative.*;

@CsClass(CS_CLASS_NAME)
public class TerrainMeshState extends SpatialState {

    public TerrainMeshState() {
    }

    public TerrainMeshState(long ref) {
        super(ref);
    }

    public void setTerrainMesh(ITerrainMesh terrainMesh) {
        nSetTerrainMesh(getRef(), terrainMesh != null ? terrainMesh.getRef() : 0);
    }

    public ITerrainMesh getTerrainMesh() {
        return (ITerrainMesh) nGetTerrainMesh(getRef());
    }

    public void setLayerMask(TerrainLayerMask layerMask) {
        nSetLayerMask(getRef(), layerMask != null ? layerMask.getRef() : 0);
    }

    public void addLayer(TerrainLayer layer) {
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

    public void setReference(SpatialState reference) {
        nSetReference(getRef(), reference != null ? reference.getRef() : 0);
    }

    public SpatialState getReference() {
        return (SpatialState) nGetReference(getRef());
    }

}
