class Board:
	""" Board represented as a list of list. 
		Basically this is just a tree node """

	def __init__(self, state, parent=None, action=None, depth=0, path_cost=0):
		self.state = state
		self.parent = parent
		self.depth = self.parent.depth + 1 if self.parent else 0
		self.path_cost = path_cost
		self.action = action
		self.current_position = self.find("mouse")

	def path(self):
		"Create a list of nodes from the root to this node"
		x, path = self, [self.action]
		while True:
			if not x.parent:
				break
			path.append(x.parent.action)
			x = x.parent
		return path

	def find(self, type):
		"Finds position of mouse or meat in this board state. type='mouse' or type='meat' returns (x,y) position"
		for x in self.state:
			for y in x:
				if "M" == y and type == "mouse":
					return (self.state.index(x), x.index(y))
				if "." == y and type == "meat":
					return (self.state.index(x), x.index(y))
		return False
		
	def __str__(self):
		"Show the board and mouse position"
		for y in self.state:
			print "".join(y)
		print "Pos: ", self.current_position
		print "Depth", self.depth
		return ""

	def __eq__(self, other): 
		"Check if two state equal, prevents the duplicates"
		return self.current_position == other.current_position