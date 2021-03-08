#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

uniform mat4 spc_ShadowMapViewProjectionMatrix[3];


void main()
{
	for (int i=0; i<3; i++)
	{
		gl_Layer = i;

		gl_Position = spc_ShadowMapViewProjectionMatrix[i] * gl_in[0].gl_Position;
		EmitVertex();

		gl_Position = spc_ShadowMapViewProjectionMatrix[i] * gl_in[1].gl_Position;
		EmitVertex();

		gl_Position = spc_ShadowMapViewProjectionMatrix[i] * gl_in[2].gl_Position;
		EmitVertex();

		EndPrimitive();

	}

}

