package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.CsMesh;
import org.cryo.core.graphics.material.IMaterial;

import static org.cryo.core.entity.CsStaticMeshStateNative.*;

@CsClass("cs::csStaticMeshState")
public class CsStaticMeshState extends CsSpatialState
{

	public CsStaticMeshState()
	{
	}

	public CsStaticMeshState(long ref)
	{
		super(ref);
	}

	public void setMesh (CsMesh mesh)
	{
		nSetMesh(getRef(), refOf(mesh));
	}

	public CsMesh getMesh ()
	{
		return (CsMesh)nGetMesh(getRef());
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
