package org.crimsonedge.demo.scene;

import org.crimsonedge.core.csObjectRegistry;
import org.crimsonedge.core.entity.csEntity;
import org.crimsonedge.core.entity.csStaticMeshState;
import org.crimsonedge.core.graphics.*;
import org.crimsonedge.core.graphics.material.IMaterial;
import org.crimsonedge.core.math.csVector2f;
import org.crimsonedge.core.math.csVector3f;
import org.crimsonedge.core.math.csVector4f;
import org.crimsonedge.core.resource.csAssetManager;

import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class TestEntity extends csEntity {

    static byte[] textureRGB(int width, int height) {
        byte[] res = new byte[width * height * 3];
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                int o = (i*width+j) * 3;

                res[o] = (byte)0x00;
                res[o+1] = (byte)0x00;
                res[o+2] = (byte)0xff;
            }
        }
        return res;
    }


    public TestEntity() {
        super ();

        IDevice device = csObjectRegistry.get(IDevice.class);

        ITexture2D.Descriptor desc = new ITexture2D.Descriptor();
        desc.format = EPixelFormat.RGB;
        desc.width = 256;
        desc.height = 256;
        desc.mipMaps = false;
        desc.multiSamples = 1;

        ITexture2D txt2d = device.createTexture(desc);
        txt2d.data(0, EPixelFormat.RGB, textureRGB(desc.width, desc.height));

        IMaterial planeMaterial = csAssetManager.get(IMaterial.class, "/materials/Default.mat");
        int diffuseIdx = planeMaterial.indexOf("Diffuse");
        planeMaterial.set(diffuseIdx, txt2d);

        IRenderMesh plane = generatePlane();
        csMesh mesh = new csMesh();
        int slot = mesh.addMaterialSlot("Default", planeMaterial);
        mesh.addSubMesh(plane, slot);

        csStaticMeshState staticMeshState = new csStaticMeshState();

        staticMeshState.setMesh(mesh);
        staticMeshState.setCastShadow(false);
        attach(staticMeshState);
        attach(new MyState());

    }

    private IRenderMesh generatePlane() {
        IRenderMeshGeneratorFactory iRenderMeshGeneratorFactory = csObjectRegistry.get(IRenderMeshGeneratorFactory.class);
        IRenderMeshGenerator generator = iRenderMeshGeneratorFactory.create();

        List<csVector4f> vertices = Stream.of(
                new csVector4f(-1.0f, -1.0f, 0.0f, 1.0f),
                new csVector4f(-1.0f, 1.0f, 0.0f, 1.0f),
                new csVector4f(1.0f, -1.0f, 0.0f, 1.0f),
                new csVector4f(1.0f, 1.0f, 0.0f, 1.0f)
        ).collect(Collectors.toList());
        List<csVector3f> normals = Stream.of(
                new csVector3f(0.0f, 1.0f, 0.0f),
                new csVector3f(0.0f, 1.0f, 0.0f),
                new csVector3f(0.0f, 1.0f, 0.0f),
                new csVector3f(0.0f, 1.0f, 0.0f)
        ).collect(Collectors.toList());
        List<csVector2f> uvs = Stream.of(
                new csVector2f(0.0f, 0.0f),
                new csVector2f(0.0f, 1.0f),
                new csVector2f(1.0f, 0.0f),
                new csVector2f(1.0f, 1.0f)
        ).collect(Collectors.toList());
        List<Integer> indices = Stream.of(0, 1, 3, 0, 3, 2, 0, 3, 1, 0, 2, 3).collect(Collectors.toList());
        generator.setVertices4f(vertices);
        generator.setUV02f(uvs);
        generator.setIndices(indices);
        return generator.generate();
    }
}
