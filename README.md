# Solving Sudoku with Dancing Links

This project aims to solve 9x9 Sudoku puzzles efficiently using the **Dancing Links algorithm**, which is a specific method for solving **Exact Cover Problems**.


## Project Overview

### Initial Approach 
The project started with a basic **backtracking algorithm**, which is a brute-force approach to solving Sudoku puzzles. While functional, this method can be slow and inefficient for larger or more complex puzzles.

### Optimization with Heuristics
To improve performance, I introduced **heuristics** that help reduce the search space by eliminating possibilities based on the constraints of Sudoku: rows, columns, and subgrids. This narrowed down the valid options at each step, enabling the algorithm to make faster decisions.

### Heuristic Techniques
I experimented with advanced Sudoku solving techniques, such as **hidden singles** and **hidden doubles**, in an attempt to further optimize performance. However, these techniques did not significantly improve the algorithm’s performance.

### Dancing Links Algorithm
The breakthrough came when I encountered the **Dancing Links algorithm**, a more advanced method for efficiently solving exact cover problems like Sudoku. This algorithm uses a technique that dramatically reduces time complexity, offering a significant performance boost.

## Tested with the "World's Hardest Sudoku"
This algorithm was tested with the **"World's Hardest Sudoku Puzzle"**, as described in [The Telegraph](https://www.telegraph.co.uk/news/science/science-news/9359579/Worlds-hardest-sudoku-can-you-crack-it.html). Below is the puzzle used for testing:

```

vector<vector<int>> puzzle = {
  {8,0,0,0,0,0,0,0,0},
  {0,0,3,6,0,0,0,0,0},
  {0,7,0,0,9,0,2,0,0},
  {0,5,0,0,0,7,0,0,0},
  {0,0,0,0,4,5,7,0,0},
  {0,0,0,1,0,0,0,3,0},
  {0,0,1,0,0,0,0,6,8},
  {0,0,8,5,0,0,0,1,0},
  {0,9,0,0,0,0,4,0,0}
};

```
This puzzle served as a rigorous test of the algorithm's efficiency and performance.

## Handling Edge Cases
An interesting challenge arose when I made a simple modification—removing the 8 from the top-left corner of the puzzle. This change caused a significant slowdown for the backtracking algorithm, but the Dancing Links algorithm handled the modified puzzle with ease, showcasing its robustness and speed.

```

vector<vector<int>> puzzle = {
  {0,0,0,0,0,0,0,0,0},
  {0,0,3,6,0,0,0,0,0},
  {0,7,0,0,9,0,2,0,0},
  {0,5,0,0,0,7,0,0,0},
  {0,0,0,0,4,5,7,0,0},
  {0,0,0,1,0,0,0,3,0},
  {0,0,1,0,0,0,0,6,8},
  {0,0,8,5,0,0,0,1,0},
  {0,9,0,0,0,0,4,0,0}
};

```
This modified puzzle offered a greater challenge to the backtracking algorithm, demonstrating how small changes in input can significantly affect performance.
