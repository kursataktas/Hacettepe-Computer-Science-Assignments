from Queue import *
from path_finder import PathFinder

class Search:

	@staticmethod
	def bfs(init_state, goal_position):
		P = PathFinder(init_state)
		Q = Queue(maxsize=0)

		Q.put(init_state)
		expanded = -1
		while not Q.empty():
			board = Q.get()
			expanded = expanded + 1
			if board.current_position == goal_position:
				s = ""
				for i in list(reversed(board.path()[:-1])):
					s = s + i + "\n"
				s = s + "WIN\n"
				s = s + "Expanded node count: " + str(expanded)
				return s
			for successor in P.successor(board):
				Q.put(successor)
		return "LOSE"


	@staticmethod
	def dfs_limited(init_state, goal_position, depth_limit):
		P = PathFinder(init_state)
		Q = LifoQueue(maxsize=0)

		Q.put(init_state)
		while not Q.empty():
			board = Q.get()
			if board.current_position == goal_position:
				s = ""
				for i in list(reversed(board.path()[:-1])):
					s = s + i + "\n"
				s = s + "WIN\n"
				s = s + "Expanded node count: x"
				return s

			# Check depth before expand
			if depth_limit > board.depth:
				for successor in P.successor(board):
					Q.put(successor)
		return "LOSE"

	@staticmethod
	def iddfs(init_state, goal_position):
		for i in range(1, 100):
			found = Search.dfs_limited(init_state, goal_position, i)
			if found != "LOSE":
				return found
		return "LOSE"

	@staticmethod
	def astar(init_state, goal_position):
		P = PathFinder(init_state)
		Q = PriorityQueue(maxsize=0)

		Q.put((0, init_state))

		expanded = -1
		while not Q.empty():
			board = Q.get()[1]
			expanded = expanded + 1

			if board.current_position == goal_position:
				s = ""
				for i in list(reversed(board.path()[:-1])):
					s = s + i + "\n"
				s = s + "WIN\n"
				s = s + "Expanded node count: " + str(expanded)
				return s

			# total cost used as a priority measurement
			for successor in P.successor(board):
				total_cost = successor.depth + Search.manhattan_distance(successor.current_position, goal_position)
				Q.put((total_cost, successor))
		return "LOSE"

	@staticmethod
	def manhattan_distance(pos1, pos2):
		return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])