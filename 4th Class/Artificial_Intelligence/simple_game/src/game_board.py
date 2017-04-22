from copy import deepcopy
import sys

class GameBoard():
	""" Board represented as a list of list. 
		Basically this is just a tree node """

	def __init__(self, state, parent=None, action=None, score=0):
		self.state = state
		self.parent = parent
		self.depth = self.parent.depth + 1 if self.parent else 0
		self.action = action
		self.current_position = self.find("mouse")
		self.cat_position = self.find("cat")
		self.score = score

	def successor(self):
		"Finds successors of current board returns list of Board objects"
		pos = self.current_position
		fringe = []

		north_pos = (pos[0] - 1, pos[1])
		south_pos = (pos[0] + 1, pos[1])
		east_pos = (pos[0], pos[1] + 1)
		west_pos = (pos[0], pos[1] - 1)

		if self.state[ west_pos[0] ][ west_pos[1] ] != "#":
			score = self.evaluate_score(self, west_pos)
			fringe.append(	GameBoard(self.move_mouse(self.state, pos, west_pos), 
								self, "West", score)
			)

		# Stop
		score = self.evaluate_score(self, pos)
		fringe.append(	GameBoard(self.move_mouse(self.state, pos, pos), 
							self, "Stop", score)
		)

		if self.state[ east_pos[0] ][ east_pos[1] ] != "#":
			score = self.evaluate_score(self, east_pos)
			fringe.append(	GameBoard(self.move_mouse(self.state, pos, east_pos), 
								self, "East", score)
			)

		if self.state[ north_pos[0] ][ north_pos[1] ] != "#":
			score = self.evaluate_score(self, north_pos)
			fringe.append(	GameBoard(self.move_mouse(self.state, pos, north_pos), 
								self, "North", score)
			)

		if self.state[ south_pos[0] ][ south_pos[1] ] != "#":
			score = self.evaluate_score(self, south_pos)
			fringe.append(	GameBoard(self.move_mouse(self.state, pos, south_pos), 
								self, "South", score)
			)

		return fringe

	def move_mouse(self, state, old_pos, new_pos):
		new_state = deepcopy(state)
		new_state[old_pos[0]][old_pos[1]] = ' '
		new_state[new_pos[0]][new_pos[1]] = 'M'
		return new_state

	def move_cat(self):
		pos = self.cat_position
		north_pos = (pos[0] - 1, pos[1])
		south_pos = (pos[0] + 1, pos[1])
		east_pos = (pos[0], pos[1] + 1)
		west_pos = (pos[0], pos[1] - 1)

		if self.state[ west_pos[0] ][ west_pos[1] ] != "#" and self.parent.parent.cat_position != west_pos:
			new_pos = west_pos
		elif self.state[ east_pos[0] ][ east_pos[1] ] != "#" and self.parent.parent.cat_position != east_pos:
			new_pos = east_pos
		elif self.state[ north_pos[0] ][ north_pos[1] ] != "#" and self.parent.parent.cat_position != north_pos:
			new_pos = north_pos
		elif self.state[ south_pos[0] ][ south_pos[1] ] != "#" and self.parent.parent.cat_position != south_pos:
			new_pos = south_pos

		self.state[pos[0]][pos[1]] = ' '
		if(self.state[new_pos[0]][new_pos[1]] == 'M'):
			self.state[new_pos[0]][new_pos[1]] = 'C'
		else:
			self.state[new_pos[0]][new_pos[1]] = 'C'

	def evaluate_score(self, board, new_pos):
		if board.state[new_pos[0]][new_pos[1]] == '.':
			return board.score + 10 - 1
		if board.state[new_pos[0]][new_pos[1]] == 'C':
			return -sys.maxint + 1
		else:
			return board.score -1

	def check_game_end(self):
		if not self.find("meat"):
			return True
		else:
			return False

	def find(self, type):
		"Finds position of mouse or meat in this board state. type='mouse' or type='meat' returns (x,y) position"
		for x in self.state:
			for y in x:
				if "M" == y and type == "mouse":
					return (self.state.index(x), x.index(y))
				if "C" == y and type == "cat":
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
		print "Score", self.score
		return ""