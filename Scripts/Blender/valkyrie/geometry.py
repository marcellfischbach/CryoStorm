
import valkyrie
import struct



class GeometryWriter:
	
	GT_GeometryMesh = 0
	GT_GeometryCollection = 1
	GT_GeometryLOD = 2
	
	DM_Internal = 0
	DM_Exteral = 1
	
	def __init__(self):
		self.stream = []
		self.material_map = {}

	def write(self, multi_mesh):
		self.stream += struct.pack('<I', GeometryWriter.GT_GeometryMesh)
		self.stream += struct.pack('<ffff', 1.0, 0.0, 0.0, 0.0);
		self.stream += struct.pack('<ffff', 0.0, 1.0, 0.0, 0.0);
		self.stream += struct.pack('<ffff', 0.0, 0.0, 1.0, 0.0);
		self.stream += struct.pack('<ffff', 0.0, 0.0, 0.0, 1.0);
		
		self.stream += struct.pack('<I', GeometryWriter.DM_Internal)
	
		self._write_string('Mesh')
		
		# export the materials that are stored within this static mesh
		self._prepare_material_map(multi_mesh)
		self._write_material_names()
		
		
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
		
	