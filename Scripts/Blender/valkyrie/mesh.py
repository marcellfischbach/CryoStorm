
import valkyrie
import bpy
import struct
import sys

class MeshData:

	def __init__(self, unique, material_index, material_name):
		self.material_index = material_index
		self.material_name = material_name
		self.vertices = valkyrie.UniqueList(unique)
		self.trigons = valkyrie.UniqueList(unique)
		self.bboxMin = [sys.float_info.max, sys.float_info.max, sys.float_info.max]
		self.bboxMax = [-sys.float_info.max, -sys.float_info.max, -sys.float_info.max]
		
		self.has_position = True
		self.has_normal = True
		self.has_texture = False
	
	def merge_mesh(self, mesh, matrix):
		mmap = {}
		i = 0
		self.has_position = self.has_position or mesh.has_position
		self.has_normal = self.has_normal or mesh.has_normal
		self.has_texture = self.has_texture or mesh.has_texture
		
		for vert in mesh.vertices.list:
			nv = vert.mult_by_matrix(matrix)
			
			self.bboxMin[0] = min(self.bboxMin[0], nv.x)
			self.bboxMin[1] = min(self.bboxMin[1], nv.y)
			self.bboxMin[2] = min(self.bboxMin[2], nv.z)
			self.bboxMax[0] = max(self.bboxMax[0], nv.x)
			self.bboxMax[1] = max(self.bboxMax[1], nv.y)
			self.bboxMax[2] = max(self.bboxMax[2], nv.z)
			
			ni = self.vertices.add(nv)
			mmap[i] = ni
			i+=1
		
		for trig in mesh.trigons.list:
			
			ni0 = mmap[trig.v0]
			ni1 = mmap[trig.v1]
			ni2 = mmap[trig.v2]
			
			trig = valkyrie.Trigon()
			trig.v0 = ni0
			trig.v1 = ni1
			trig.v2 = ni2
			self.trigons.add(trig)
			


		
			
			
	def debug(self):
		print ("    Data: ", self.material_index)
			
		print ("      Vertices: ", len(self.vertices.list))
#		for v in self.vertices.list:
#			print ("  Co: %.2f %.2f %.2f  Norm: %.2f %.2f %.2f" % (v.x, v.y, v.z, v.nx, v.ny, v.nz))
			
		print ("      Trigon: ", len(self.trigons.list))


class Mesh:
	
	def __init__(self):
		self.mesh_datas = []
		
	
	def debug(self):
		print("Mesh:", len(self.mesh_datas))
		for md in self.mesh_datas:
			md.debug()
			
	
	
	def setup_from_blender_mesh(self, obj, mesh):
		current_md = None
		
		uv_layer = Mesh._find_active_uv_layer (mesh)
		
		for poly in mesh.polygons:
			if current_md == None or current_md.material_index != poly.material_index:
				current_md = self._get_mesh_data(obj, poly.material_index)
				current_md.has_texture = uv_layer != None
			mmap = [[0, 1, 2], [0, 2, 3]]
			num_trigs = poly.loop_total - 2
			for i in range(num_trigs):
				i0 = self._insert_vertex_at_loop(mesh,poly.loop_start + mmap[i][0], poly, uv_layer, current_md) 
				i1 = self._insert_vertex_at_loop(mesh,poly.loop_start + mmap[i][1], poly, uv_layer, current_md) 
				i2 = self._insert_vertex_at_loop(mesh,poly.loop_start + mmap[i][2], poly, uv_layer, current_md) 
				
				trig = valkyrie.Trigon()
				trig.v0 = i0
				trig.v1 = i2
				trig.v2 = i1
				
				current_md.trigons.add(trig)

				
				current_md.trigons.add(trig)
				
	def _get_material_name(self, obj, material_index):
		return obj.material_slots[material_index].material.name

	def _get_mesh_data(self, obj, material_index):
		for md in self.mesh_datas:
			if md.material_index == material_index:
				return md
		
		material_name = self._get_material_name(obj, material_index)
		md = MeshData(True, material_index, material_name)
		self.mesh_datas.append(md)
		return md

			
	def _insert_vertex_at_loop(self, mesh, loop_idx, poly, uv_layer, mesh_data):
		loop = mesh.loops[loop_idx]
		vertex = mesh.vertices[loop.vertex_index]
					
		v = valkyrie.Vertex ()
		v.x = vertex.co.x
		v.y = vertex.co.y
		v.z = vertex.co.z
		
		if poly.use_smooth:
			v.nx = vertex.normal.x
			v.ny = vertex.normal.y
			v.nz = vertex.normal.z
		else:
			v.nx = poly.normal.x
			v.ny = poly.normal.y
			v.nz = poly.normal.z
		
		if uv_layer != None:
			uv_loop = uv_layer.data[loop_idx]
			v.tu = uv_loop.uv[0]
			v.tv = uv_loop.uv[1]
		else:
			v.tu = 0.0
			v.tv = 0.0
		
		idx = mesh_data.vertices.add(v)
		return idx
		

			
	def _find_active_uv_layer(mesh):
		# first find the uv_texture that's active for rendering
		for uv_texture in mesh.uv_textures:
			if uv_texture.active_render == True:
				return Mesh._find_active_uv_layer_by_name(mesh, uv_texture.name)
				
		return None

	def _find_active_uv_layer_by_name(mesh, name):
		for uv_layer in mesh.uv_layers:
			if uv_layer.name == name:
				return uv_layer
		return  None
	

class MultiMesh:
	def __init__(self):
		self.mesh_datas = []

	def _create_material_lod_key(material_name, lod_id):
		return material_name + "@@@" + str(lod_id)

	def prepare_meshes(self, num_lods):
		per_lod_and_materials = { }
		
		for obj in bpy.context.scene.objects:
			data = obj.data
			if obj.name[0:2] == "PX":
				continue
				
			if isinstance(data, bpy.types.Mesh):
				for lID in range(num_lods):
					# check if the object is visible in the given LOD-layer
					if not obj.layers[lID]:
						continue
					
					# create a full valkyrie mesh
					mesh = valkyrie.Mesh()
					mesh.setup_from_blender_mesh(obj, data)
					
					# delegate the meshes with the different materials onto the per_lod_materials meshes
					for mesh_data in mesh.mesh_datas:
						material_name = mesh_data.material_name
						lod_mat_key = MultiMesh._create_material_lod_key(material_name, lID)
						
						per_material_and_lod_mesh_data = None
						if lod_mat_key in per_lod_and_materials:
							per_material_and_lod_mesh_data = per_lod_and_materials[lod_mat_key]
						else:
							#repurpose the material_index as the LOD-ID
							per_material_and_lod_mesh_data = valkyrie.MeshData(False, lID, material_name)
							per_lod_and_materials[lod_mat_key] = per_material_and_lod_mesh_data
						
						per_material_and_lod_mesh_data.merge_mesh(mesh_data, obj.matrix_world)
	
		for key in per_lod_and_materials:
			mesh_data = per_lod_and_materials[key]
			self.mesh_datas.append(mesh_data)
			
			
			
class MultiMeshWriter:
	
	DT_Byte = 0
	DT_UnsignedByte = 1
	DT_Short = 2
	DT_UnsignedShort = 3
	DT_Int = 4
	DT_UnsignedInt = 5
	DT_Float = 6
	DT_Double = 7
	
	
	VST_Position = 0
	VST_Normal = 1
	VST_Color = 2
	VST_Tangent = 3
	VST_BiNormal = 4

	VST_Position1 = 5
	VST_Normal1 = 6
	VST_Color1 = 7
	VST_Tangent1 = 8
	VST_BiNormal1 = 9

	VST_TexCoord0 = 10
	VST_TexCoord1 = 11
	VST_TexCoord2 = 12
	VST_TexCoord3 = 13
	VST_TexCoord4 = 14
	VST_TexCoord5 = 15
	VST_TexCoord6 = 16
	VST_TexCoord7 = 17

	VST_BoneIndex = 18
	VST_BoneWeight = 19


	def __init__(self):
		self.stream = []
		self.material_map = { }
		
		
	def write (self, multi_mesh):
		self._prepare_material_map(multi_mesh)
		self._write_material_names()
		
		self._write_meshes (multi_mesh)
		
	def _write_meshes(self, multi_mesh):
		self.stream += struct.pack('<I', len(multi_mesh.mesh_datas))
		
		for mesh_data in multi_mesh.mesh_datas:
			self._write_mesh(mesh_data)
			
	def _write_mesh(self, mesh_data):
		mat_idx = self.material_map[mesh_data.material_name]
		self.stream += struct.pack('<I', mat_idx)
		# mesh_data.material_index is really the LOD-ID of the mesh
		self.stream += struct.pack('<I', mesh_data.material_index)
		
		self._write_vertex_declaration(mesh_data)
		self._write_vertices (mesh_data)
		self._write_indices (mesh_data)
		self._write_bounding_box(mesh_data)
		
	def _write_vertex_declaration(self, mesh_data):
		num_decl = 0
		stride = 0
		if mesh_data.has_position:
			num_decl += 1
			stride += 3 * 4 # 3 floats
		if mesh_data.has_normal:
			num_decl += 1
			stride += 3 * 4 # 3 floats
		if mesh_data.has_texture:
			num_decl += 1
			stride += 2 * 4 # 2 floats
			
		self.stream += struct.pack('<I', num_decl)
		
		offset = 0
		if mesh_data.has_position:
			self.stream += struct.pack('<IIIIII', MultiMeshWriter.VST_Position, MultiMeshWriter.DT_Float, 3, offset, stride, 0)
			offset += 3 * 4
		if mesh_data.has_normal:
			self.stream += struct.pack('<IIIIII', MultiMeshWriter.VST_Normal, MultiMeshWriter.DT_Float, 3, offset, stride, 0)
			offset += 3 * 4
		if mesh_data.has_texture:
			self.stream += struct.pack('<IIIIII', MultiMeshWriter.VST_TexCoord0, MultiMeshWriter.DT_Float, 2, offset, stride, 0)
			offset += 2 * 4
			
	def _write_vertices(self, mesh_data):
		self.stream += struct.pack('<I', len(mesh_data.vertices.list))
		# only export into 1 stream
		self.stream += struct.pack('<I', 1)
		stride = 0
		if mesh_data.has_position:
			stride += 3 * 4
		if mesh_data.has_normal:
			stride += 3 * 4
		if mesh_data.has_texture:
			stride += 2 * 4
		
		self.stream += struct.pack('<I', stride)
		
		for vert in mesh_data.vertices.list:
			if mesh_data.has_position:
				self.stream += struct.pack('<fff', vert.x, vert.y, vert.z)
			if mesh_data.has_normal:
				self.stream += struct.pack('<fff', vert.nx, vert.ny, vert.nz)
			if mesh_data.has_texture:
				self.stream += struct.pack('<ff', vert.tu, vert.tv)
			
	def _write_indices(self, mesh_data):
		num_ind = len(mesh_data.trigons.list) * 3
		self.stream += struct.pack('<I', num_ind)
		
		num_vertices = len(mesh_data.vertices.list)
		is32bit = num_vertices >= 65536
		if is32bit:
			self.stream += struct.pack('<I', MultiMeshWriter.DT_UnsignedInt)
		else:
			self.stream += struct.pack('<I', MultiMeshWriter.DT_UnsignedShort)
			
		for trig in mesh_data.trigons.list:
			if is32bit:
				self.stream += struct.pack('<III', trig.v0, trig.v1, trig.v2)
			else:
				self.stream += struct.pack('<HHH', trig.v0, trig.v1, trig.v2)

	def _write_bounding_box(self, multi_mesh):
		self.stream += struct.pack('<ffffff', multi_mesh.bboxMin[0], multi_mesh.bboxMin[1], multi_mesh.bboxMin[2], multi_mesh.bboxMax[0], multi_mesh.bboxMax[1], multi_mesh.bboxMax[2])
		
		
	def _prepare_material_map(self, multi_mesh):
		
		i = 0
		for mesh_data in multi_mesh.mesh_datas:
			mat_name = mesh_data.material_name
			if not mat_name in self.material_map:
				self.material_map[mat_name] = i
				i+=1
		
		
	def _write_material_names(self):

		self.stream += struct.pack('<I', len(self.material_map))
		for material_name in self.material_map:
			self._write_string(material_name)
	
	def _write_string(self, string):
		_string = bytes(string, 'latin1')
		self.stream += struct.pack("<I%dsb" % (len(_string)), len(_string)+1, _string, 0)
		
	