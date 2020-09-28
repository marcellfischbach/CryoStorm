

import valkyrie
import bpy
import struct
import sys

class Collision:
	
	PGT_Sphere = 0
	PGT_CylinderX = 1
	PGT_CylinderY = 2
	PGT_CylinderZ = 3
	PGT_CapsuleX = 4
	PGT_CapsuleY = 5
	PGT_CapsuleZ = 6
	PGT_Box = 7
	PGT_TriMesh = 10
	
	def __init__(self):
		self.stream = []
		
	
	def prepare(self):
		num_shapes = 0
		for obj in bpy.context.scene.objects:
			data = obj.data
			if not isinstance(data, bpy.types.Mesh):
				continue
	
			if obj.name[0:2] != "PX":
				continue
			
			if obj.name[0:6] == "PX_Box":
				num_shapes += 1
				self._collect_box(obj)
				
		stream = self.stream
		self.stream = []
		self.stream += struct.pack('<I', num_shapes)
		self.stream += stream
		
				
	def _write_transformation (self, obj):
		mat = obj.matrix_world
		print("Matrix: ", mat[0])
		self.stream += struct.pack('<ffff', mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
		self.stream += struct.pack('<ffff', mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
		self.stream += struct.pack('<ffff', mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
		self.stream += struct.pack('<ffff', mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
				
	def _collect_box(self, obj):
		dim = self._calc_dimensions(obj.data)
		
		self.stream += struct.pack('<I', Collision.PGT_Box)
		self._write_transformation(obj)
		self.stream += struct.pack('<fff', dim[0], dim[1], dim[2])
		print ("Box dim: %f %f %f\n" % (dim[0], dim[1], dim[2]))
		
		
		

	def _calc_dimensions(self, mesh):
		bboxMin = [sys.float_info.max, sys.float_info.max, sys.float_info.max]
		bboxMax = [-sys.float_info.max, -sys.float_info.max, -sys.float_info.max]
		for vert in mesh.vertices:
			v = vert.co
			bboxMin[0] = min(bboxMin[0], v.x)
			bboxMin[1] = min(bboxMin[1], v.y)
			bboxMin[2] = min(bboxMin[2], v.z)
			bboxMax[0] = max(bboxMax[0], v.x)
			bboxMax[1] = max(bboxMax[1], v.y)
			bboxMax[2] = max(bboxMax[2], v.z)
			
		return [bboxMax[0] - bboxMin[0], bboxMax[1] - bboxMin[1], bboxMax[2] -bboxMin[2]]
		
		
		
		