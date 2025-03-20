package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.Mesh;
import org.cryo.core.graphics.material.IMaterial;
import org.cryo.core.graphics.material.Material;

import static org.cryo.core.entity.StaticMeshStateNative.*;

@CsClass("cs::csStaticMeshState")
public class StaticMeshState extends SpatialState
{

	public StaticMeshState()
	{
	}

	public StaticMeshState(long ref)
	{
		super(ref);
	}

	public void setMesh (Mesh mesh)
	{
		nSetMesh(getRef(), refOf(mesh));
	}

	public Mesh getMesh ()
	{
		return (Mesh)nGetMesh(getRef());
	}

	public void setMaterial (int idx, IMaterial material)
	{
		nSetMaterial(getRef(), idx, refOf(material));
	}


	public IMaterial getMaterial(int idx)
	{
		return (IMaterial)nGetMaterial(getRef(), idx);
	}

	public void setReceiveShadow (boolean receiveShadow)
	{
		nSetReceiveShadow(getRef(), receiveShadow);
	}

	public boolean isReceiveShadow ()
	{
		return nIsReceiveShadow(getRef());
	}

	public void setCastShadow (boolean castShadow)
	{
		nSetCastShadow(getRef(), castShadow);
	}

	public boolean isCastShadow ()
	{
		return nIsCastShadow(getRef());
	}

}
