from Queue import *
from game_board import GameBoard
from copy import deepcopy
import sys

class Minimax:

	def __init__(self, game_board, depth):
		self.game_board = game_board
		self.depth = depth
		self.i = 0

	def start(self):
		path = Queue(maxsize=0)	

		x = self.max_play(self.game_board)
		path.put(x)	
		x.move_cat()

		i = 0
		while i < 10:
			i = i + 1
			x.depth = 0
			x = self.max_play(x)
			path.put(x)
			x.move_cat()
		s = ""
		while not path.empty():
			s = s + path.get().action + "\n"
		return s
		
	def max_play(self, board):
		if board.check_game_end() or board.depth >= self.depth:
			return board

		successors = board.successor()
		max_score = -sys.maxint

		
		for s in successors:
			max_board = self.min_play(s)
			if max_board.score > max_score:
				best = max_board
				max_score = max_board.score
		return best

	def min_play(self, board):
		if board.check_game_end() or board.depth >= self.depth:
			return board

		successors = board.successor()
		min_score = sys.maxint

		for s in successors:
			min_board = self.max_play(s)
			if min_board.score < min_score:
				best = min_board
				min_score = min_board.score
		return best




		