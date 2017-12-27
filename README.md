# CS325_Analysis_of_Algorithms
Analysis of Algorithms Group Assignment

#### Each assignment folder contains a pdf with the description of the group assignment, the implementation of algorithms, and test cases.

#### Each test case contains input.txt and maybe data files, and a test file that contains the correct answer to the test case. To run the program, copy the selected test case to the program's directory, and run the program using g++. The result of the algorithm will be outputted to output.txt.

## Assignment 1 - find kth smallest element
#### Description
The `input.txt` contains 3 numbers. First is m, the number of data files, second is n, the number of numbers each file contains. The third is k. The algorithm should return the kth smallest element of all numbers in data files.

The data files contain 32-bit numbers, so if the file has n numbers, the file size should be 4n Bytes.

#### Solution
Use concepts of binary search. Start from the first file, binary search number x. For each search, search all other files so that the numbers are just below x. Add all the index together and compare to k. If it is lower, move the low to this index, otherwise, move the high to this index. If the high is 1 above low, yet the total index is not equal to k, move to the next files and repeat, until the number is found.

#### Runtime
Runtime of solution should be O(m^2*(logn)^2). It takes logn steps to finish searching one file, and each search in one file has at most m*logn searches in other files to determine total index value to compare to k. with m files to binary search through, the total should be m * logn & (m*logn) = m^2*(logn)^2.

## Assignment 2 - Vankin's Mile
#### Description
The `input.txt` contains numbers that describes the game board. The first line contains a value n, the number of rows and columns. The next n lines each contains n numbers that represent an n-by-n grid of the board.

Vankin's Mile is a game where player starts on a grid of the board, and moves right or down, until reaching the edge of the board. The score is the sum of all values on the paths of the player. The goal is to reach the highest possible score. The algorithm should return the maximum possible score from the board described in `input.txt`.

#### solution
Use dynamic programming. From the bottom row to the top row, scan from the right column to the left, calculate max score for each space. The max score of each space in the grid should be = (value on that space) + max(max score of space to the right, max score of space below). The implementation is optimized so it only uses O(n) space instead of O(n^2).

#### Runtime
Runtime of solution should be O(n^2). Each space is checked once, so the runtime is n^2.

## Assignment 3 - Minimum Spanning Tree
#### Description
The `input.txt` contains adjacent matrix that describes the complete graph. The first line contains a number n. The next n lines each contains n numbers, which describes the n-by-n adjacent matrix.

The algorithm should calculate the 1st, 2nd and 3rd minimum spanning tree.

#### Solution
There are 3 algorithms that can be used. Boruvka's algorithm is used for this group assignment. The algorithm recursively do the following until minimum spanning tree is formed:
1. labels each vertex, so that all connected components have the same labels.
2. For each forest, choose a safe edge and connect it.

To get the 2nd and 3rd spanning tree, try disconnecting each edge. For each disconnection, the minimum spanning tree is split into 2 forests. Try each edge that connects the 2 forests that is not in the original minimum spanning tree, and store the 2 lowest minimum spanning tree weight inside an array. If any value is smaller than one of the 2 values in the array, replace it.

#### Runtime
The total runtime to find 1st, 2nd and 3rd spanning tree should be O(E*V). The runtime of Baruvka's algorithm to find first minimum spnaning tree is O(ElogV), where E is the number of edges and V is the number of vertex. For second and third spanning tree, each edge disconnected, at most E edges will be checked. Since there are only (V-1) edges in a minimum spanning tree, the total runtime should be E*(V-1) = EV - E. So the total runtime equals O(ElogV) + O(EV - E) = O(E*V).

## Assignment 4 - Light and Switches Problems
#### Description
Each light is connected to 2 switches, the algorithm should output "yes" if the all lights can be turned off, "no" otherwise. A 2-CNF satisfiability solver is provided.

The `input.txt` contains numbers that describes the connection of light and switches, and the states of the each light. The first line contains n, the number of switches, and m, the number of lights. The 2nd line contains the state of each light: "0" means the light is off whereas "1" means the light is on. The next n lines, each contains the connections of the switches.

#### Solution
Convert the lights and switches problem to 2-CNF problems. Since the turning a switch twice means not turning it, and changing the chronological order of switching the switches does not change the results. So for each light, assume the light connects to switches S1 and S2.
- If the light is on initially, create two clauses: (S1 v S2) ^ (-S1 v -S2), which means only S1 or S2 can be switched once for it to be true.
- If the light is off initially, create two clauses: (-S1 v S2) ^ (S1 v -S2), which means S1 and S2 are both either switched or not switched.

#### Runtime
The complexity of problem reduction from lights and switches problem is O(m).
