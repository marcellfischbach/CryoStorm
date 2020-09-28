
import mathutils
import valkyrie.geometry
import valkyrie.mesh
import bpy
import struct
import collections


		
class Entry:
	
	ET_Geometry = 0
	ET_Mesh = 1
	ET_Collision = 2
	ET_Skeleton = 3
	ET_SkeletonAnimation = 4
	
	def __init__(self):
		self.name = ""
		self.type = 0
		self.offset = 0
		self.size = 0
		self.data = []
		
	def get_size(self):
		return len(self.name) + 4 + 1 + 3 * 4
	

		
class StaticMeshExporter:

	MAGIC_NUMBER = 0x4853454D

	VERSION = 1
	
	def __init__(self):
		self.entries = []
	
	def export (self, filename, num_lods):
		
		mm = self._prepare_meshes(num_lods)
		self._prepare_geometry(mm)
		self._prepare_collision()
		
		self.mesh_stream = ""
		self.file = open(filename, 'wb')
		self.file.write(struct.pack('<I', StaticMeshExporter.MAGIC_NUMBER))
		self.file.write(struct.pack('<I', StaticMeshExporter.VERSION))
		
		self.entries = sorted(self.entries, key=lambda entry: entry.type)   
		
		self._prepare_headers ()
		self._export_headers ()
		self._export_entries ()
		
		self.file.close()
		
	def _prepare_headers(self):
		# header offset
		offset = 4 + 4 + 4
		for entry in self.entries:
			offset += entry.get_size()
			
		for entry in self.entries:
			entry.offset = offset
			offset += entry.size
			
	def _export_headers(self):
		self.file.write (struct.pack('<I', len(self.entries)))
		for entry in self.entries:
			self._export_string (entry.name)
			self.file.write (struct.pack('<III', entry.type, entry.offset, entry.size))

	def _export_entries (self):
		for entry in self.entries:
			self.file.write (bytes(entry.data))
		
	def _prepare_meshes(self, num_lods):
		mm = valkyrie.MultiMesh()
		mm.prepare_meshes(num_lods)
		
		mmw = valkyrie.mesh.MultiMeshWriter()
		mmw.write(mm)
		
		e = Entry()
		e.name = "Mesh"
		e.type = Entry.ET_Mesh
		e.size = len(mmw.stream)
		e.data = mmw.stream

		self.entries.append(e)
		
		return mm
		
	def _prepare_geometry(self, multi_mesh):
		
		geom_writer = valkyrie.geometry.GeometryWriter()
		geom_writer.write(multi_mesh)
		
		e = Entry()
		e.name = "Geometry"
		e.type = Entry.ET_Geometry
		e.size = len(geom_writer.stream)
		e.data = geom_writer.stream
		
		self.entries.append(e)
		
	def _prepare_collision(self):
		collision = valkyrie.collision.Collision ()
		collision.prepare()
		
		e = Entry()
		e.name = "Collision"
		e.type = Entry.ET_Collision
		e.size = len(collision.stream)
		e.data = collision.stream
		
		self.entries.append(e)
		
		
	def _export_string(self, string):
		_string = bytes(string, 'latin1')
		self.file.write(struct.pack("<I%dsb" % (len(_string)), len(_string)+1, _string, 0))
		
	