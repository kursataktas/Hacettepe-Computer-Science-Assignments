from helper import PositionList
from board import Board
from copy import deepcopy

class PathFinder:

	def __init__(self, initial_board):
		""" visited list used for preventing loops. 
			its structure => ((x, y), depth) depth is critical here. 
			see is_exist method in helper.py """

		self.visited = PositionList()
		self.visited.append((initial_board.current_position, 0))

	def successor(self, board):
		"Finds successors of current board returns list of Board objects"
		pos = board.current_position
		fringe = []

		north_pos = (pos[0] - 1, pos[1])
		south_pos = (pos[0] + 1, pos[1])
		east_pos = (pos[0], pos[1] + 1)
		west_pos = (pos[0], pos[1] - 1)

		if board.state[ north_pos[0] ][ north_pos[1] ] != "#" and not self.visited.is_exist((north_pos, board.depth + 1)):
			fringe.append(Board(self.move_mouse(board.state, pos, north_pos), board, "North"))
			self.visited.append((north_pos, board.depth + 1))

		if board.state[ south_pos[0] ][ south_pos[1] ] != "#" and not self.visited.is_exist((south_pos, board.depth + 1)):
			fringe.append(Board(self.move_mouse(board.state, pos, south_pos), board, "South"))
			self.visited.append((south_pos, board.depth + 1))

		if board.state[ east_pos[0] ][ east_pos[1] ] != "#" and not self.visited.is_exist((east_pos, board.depth + 1)):
			fringe.append(Board(self.move_mouse(board.state, pos, east_pos), board, "East"))
			self.visited.append((east_pos, board.depth + 1))

		if board.state[ west_pos[0] ][ west_pos[1] ] != "#" and not self.visited.is_exist((west_pos, board.depth + 1)):
			fringe.append(Board(self.move_mouse(board.state, pos, west_pos), board, "West"))
			self.visited.append((west_pos, board.depth + 1))

		return fringe

	def move_mouse(self, state, old_pos, new_pos):
		new_state = deepcopy(state)
		new_state[old_pos[0]][old_pos[1]] = ' '
		new_state[new_pos[0]][new_pos[1]] = 'M'
		return new_state
