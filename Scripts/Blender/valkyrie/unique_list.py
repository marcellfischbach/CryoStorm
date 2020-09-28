
import valkyrie

class UniqueList:
	
	def __init__(self, unique):
		self.list = []
		self.unique = unique
		
	def add (self, item):
		idx = 0
		if self.unique:
			# make a function pointer
			item_equals = item.equals
			for ref in self.list:
				if item_equals(ref):
					return idx
				idx += 1
		
		idx = len(self.list)
		self.list.append(item)
		return idx
		