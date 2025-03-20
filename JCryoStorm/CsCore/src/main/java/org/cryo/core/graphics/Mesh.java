package org.cryo.core.graphics;


import org.cryo.core.CsClass;
import org.cryo.core.graphics.material.IMaterial;
import org.cryo.core.resource.IAsset;

import static org.cryo.core.graphics.MeshNative.*;

@CsClass("cs::Mesh")
public class Mesh extends IAsset
{

	public Mesh()
	{
	}

	public Mesh(long ref)
	{
		super(ref);
	}

	public void addMaterialSlot(String name, IMaterial defaultMaterial)
	{
		nAddMaterialSlot(getRef(), name, defaultMaterial != null ? defaultMaterial.getRef() : 0);
	}

	public void setDefaultMaterial(int idx, IMaterial defaultMaterial)
	{
		nSetDefaultMaterial(getRef(), idx, defaultMaterial != null ? defaultMaterial.getRef() : 0);
	}

	public int getNumberOfMaterialSlots ()
	{
		return (int)nGetNumberOfMaterialSlots(getRef());
	}

	public int indexOfMaterialSlot (String name)
	{
		return (int)nIndexOfMaterialSlot(getRef(), name);
	}

	public void addSubMesh (IRenderMesh mesh, int materialSlotIdx)
	{
		nAddSubMesh(getRef(), mesh != null ? mesh.getRef() : 0, materialSlotIdx);
	}

	public int getNumberOfSubMeshes ()
	{
		return (int)nGetNumberOfSubMeshes(getRef());
	}

}
