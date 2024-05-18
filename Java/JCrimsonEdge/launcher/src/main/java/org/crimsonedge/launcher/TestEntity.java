package org.crimsonedge.launcher;

import org.crimsonedge.core.ObjectRegistry;
import org.crimsonedge.core.entity.Entity;
import org.crimsonedge.core.entity.StaticMeshState;
import org.crimsonedge.core.graphics.IRenderMesh;
import org.crimsonedge.core.graphics.IRenderMeshGenerator;
import org.crimsonedge.core.graphics.IRenderMeshGeneratorFactory;
import org.crimsonedge.core.graphics.Mesh;
import org.crimsonedge.core.graphics.material.IMaterial;
import org.crimsonedge.core.math.Vector2f;
import org.crimsonedge.core.math.Vector3f;
import org.crimsonedge.core.math.Vector4f;
import org.crimsonedge.core.resource.AssetManager;

import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class TestEntity extends Entity {

    public TestEntity() {
        super ();

        IRenderMesh plane = generatePlane();
        IMaterial planeMaterial = AssetManager.get(IMaterial.class, "/materials/Default.mat");
        Mesh mesh = new Mesh();
        int slot = mesh.addMaterialSlot("Default", planeMaterial);
        mesh.addSubMesh(plane, slot);

        StaticMeshState staticMeshState = new StaticMeshState();
        staticMeshState.setMesh(mesh);
        staticMeshState.setCastShadow(false);
        attach(staticMeshState);
        attach(new MyState());

    }

    private IRenderMesh generatePlane() {
        IRenderMeshGeneratorFactory iRenderMeshGeneratorFactory = ObjectRegistry.get(IRenderMeshGeneratorFactory.class);
        IRenderMeshGenerator generator = iRenderMeshGeneratorFactory.create();

        List<Vector4f> vertices = Stream.of(
                new Vector4f(-1.0f, -1.0f, 0.0f, 1.0f),
                new Vector4f(-1.0f, 1.0f, 0.0f, 1.0f),
                new Vector4f(1.0f, -1.0f, 0.0f, 1.0f),
                new Vector4f(1.0f, 1.0f, 0.0f, 1.0f)
        ).collect(Collectors.toList());
        List<Vector3f> normals = Stream.of(
                new Vector3f(0.0f, 1.0f, 0.0f),
                new Vector3f(0.0f, 1.0f, 0.0f),
                new Vector3f(0.0f, 1.0f, 0.0f),
                new Vector3f(0.0f, 1.0f, 0.0f)
        ).collect(Collectors.toList());
        List<Vector2f> uvs = Stream.of(
                new Vector2f(0.0f, 0.0f),
                new Vector2f(0.0f, 1.0f),
                new Vector2f(1.0f, 0.0f),
                new Vector2f(1.0f, 1.0f)
        ).collect(Collectors.toList());
        List<Integer> indices = Stream.of(0, 1, 3, 0, 3, 2, 0, 3, 1, 0, 2, 3).collect(Collectors.toList());
        generator.setVertices4f(vertices);
        generator.setUV02f(uvs);
        generator.setIndices(indices);
        return generator.generate();
    }
}
