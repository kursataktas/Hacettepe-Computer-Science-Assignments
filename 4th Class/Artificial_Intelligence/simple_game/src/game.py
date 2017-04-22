import argparse
from board import Board
from search import Search
from minimax import Minimax
from game_board import GameBoard

# take command line args
ap = argparse.ArgumentParser()
ap.add_argument('-i')
ap.add_argument('-o')
ap.add_argument('-a')
ap.add_argument('-d')

opts = ap.parse_args()

# Construct game map from given input file
with open(opts.i) as f:
    rows = f.readlines()
board = [[y for y in x.strip()] for x in rows]

a = Board(board)
goal_position = a.find("meat")

if opts.a == "bfs":
	s = Search.bfs(a, goal_position)
if opts.a == "iddfs":
	s = Search.iddfs(a, goal_position)
if opts.a == "astar":
	s = Search.astar(a, goal_position)
if opts.a == "minimax":
	s = "Yetismedi"
with open(opts.o, 'w') as f:
	f.write(s)


