class PositionList:
	""" Visited List """

	def __init__(self):
		self.visited = [];

	def append(self, position):
		self.visited.append(position)

	def is_exist(self, position):
		for p in self.visited:
			# (x, y) is in list ?
			if position[0] == p[0]: 
				# if so, new (x, y) reached less depth than older ?
				if position[1] >= p[1]:
					return True
		return False