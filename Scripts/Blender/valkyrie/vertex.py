
import valkyrie
import mathutils
from math import radians

class Vertex:

	def __init___(self):
			self.x = 0
			self.y = 0
			self.z = 0
			
			self.nx = 0
			self.ny = 0
			self.nz = 0
			
			
			self.tu = 0
			self.tv = 0
			
	def clone(self):
		r = valkyrie.Vertex()
		r.x = self.x
		r.y = self.y
		r.z = self.z
		r.nx = self.nx
		r.ny = self.ny
		r.nz = self.nz
		r.tu = self.tu
		r.tv = self.tv
		return r
			
	def equals(self, other):
		return self.x == other.x and self.y == other.y and self.z == other.z and self.nx == other.nx and self.ny == other.ny and self.nz == other.nz and self.tu == other.tu and self.tv == other.tv
	
	def mult_by_matrix(self, matrix):
		co = mathutils.Vector ((self.x, self.y, self.z, 1.0))
		no = mathutils.Vector ((self.nx, self.ny, self.nz, 0.0))
		
		co = matrix * co
		no = matrix * no
		
		self.x = co.x;
		self.y = co.y;
		self.z = co.z;
		self.nx = no.x;
		self.ny = no.y;
		self.nz = no.z;
		
		return self
	
