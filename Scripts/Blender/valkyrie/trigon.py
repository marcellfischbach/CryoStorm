
import valkyrie

class Trigon:
	def __init__(self):
		self.v0 = -1
		self.v1 = -1
		self.v2 = -1
		
	def equals (self, other):
		return self.v0 == other.v0 and self.v1 == other.v1 and self.v2 == other.v2 or self.v0 == other.v1 and self.v1 == other.v2 and self.v2 == other.v0  or self.v0 == other.v2 and self.v1 == other.v0 and self.v2 == other.v1
