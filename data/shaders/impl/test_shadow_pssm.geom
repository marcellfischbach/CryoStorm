#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

uniform mat4 spc_ShadowMapProjectionMatrix[3];
uniform mat4 spc_ShadowMapViewMatrix[3];

in vec2 geomUV[3];

out vec2 fragUV;


void main()
{
	for (int i=0; i<3; i++)
	{
		gl_Layer = i;

		gl_Position = spc_ShadowMapProjectionMatrix[i] * spc_ShadowMapViewMatrix[i] * gl_in[0].gl_Position;
		fragUV = geomUV[0];
		EmitVertex();

		gl_Position = spc_ShadowMapProjectionMatrix[i] *spc_ShadowMapViewMatrix[i] * gl_in[1].gl_Position;
		fragUV = geomUV[1];
		EmitVertex();

		gl_Position = spc_ShadowMapProjectionMatrix[i] *spc_ShadowMapViewMatrix[i] * gl_in[2].gl_Position;
		fragUV = geomUV[2];
		EmitVertex();

		EndPrimitive();

	}

}

