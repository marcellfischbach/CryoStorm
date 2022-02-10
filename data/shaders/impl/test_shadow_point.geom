#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 ce_ShadowMapViewProjectionMatrix[6];


void main()
{
//	for (int i=0; i<6; i++)
	{
		gl_Layer = 0;
		gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[0] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
	{
		gl_Layer = 1;
		gl_Position = ce_ShadowMapViewProjectionMatrix[1] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[1] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[1] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
	{
		gl_Layer = 2;
		gl_Position = ce_ShadowMapViewProjectionMatrix[2] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[2] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[2] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
	{
		gl_Layer = 3;
		gl_Position = ce_ShadowMapViewProjectionMatrix[3] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[3] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[3] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
	{
		gl_Layer = 4;
		gl_Position = ce_ShadowMapViewProjectionMatrix[4] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[4] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[4] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
	{
		gl_Layer = 5;
		gl_Position = ce_ShadowMapViewProjectionMatrix[5] * gl_in[0].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[5] * gl_in[1].gl_Position;
		EmitVertex();
		gl_Position = ce_ShadowMapViewProjectionMatrix[5] * gl_in[2].gl_Position;
		EmitVertex();
		EndPrimitive();
	}

}

