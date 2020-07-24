# HEX

This C++ console program implements the game of Hex. It allows the user to specify
the size of the board (1-26), and then the user plays white while the computer
plays black.

This was a first effort at implementing a playing algorithm. It uses a modification of the 
Dijkstra shortest-path algorithm to find the shortest path from the computer's existing pieces
and moves along that path. This algorithm plays poorly.

This program was written for Coursera's C++ for C Programmers, Part B.

## Files

- *driver.cpp Main function. Gets the size of the board and inputs player moves. Calls methods in the HexBoard class implement functionality of the game.
- *hexboard.cpp*, *hexboard.h* Implements a simple representation of the hex board using a two-dimensional graph. Methods exist to make player moves, computer moves, and display the board.
- *matrix.cpp*, *matrix.h* Implements the underlying functional representation of the board as a graph whose nodes are edges from one playing square to another. Includes the functionality implementing the Dijkstra shortest-path algorithm (variant).
- *square.cpp*, *square.h*. Class to represent a physical square on the board.
- *node.cpp*, *node.h*. Class to represent a node of the graph (which is an edge from one square to another).
