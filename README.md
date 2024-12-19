# Blocks Rearrangement

This repository contains an algorithm for rearranging blocks using a Breadth-First Search (BFS) approach. The algorithm is implemented in C++ and is designed to find the optimal path to reach a goal state from a given initial state.

## Features
- **State and Block Structures**: Defines the structures to represent the arrangement of blocks.
- **StateSpace Class**: Manages the state of blocks and their movements.
- **Priority Queue**: Utilizes a priority queue to prioritize states nearer to the goal.
- **Heuristic Function**: Calculates how close the current state is to the goal state.
- **BFS Algorithm**: Finds the optimal path to reach the goal state.
- **Performance Measurement**: Measures the time taken to find the solution.

## File Structure
- `BlocksSearshAlgorithm/BlocksSearshAlgorithm.cpp`: Main implementation of the block rearrangement algorithm.
- `README.md`: Project description and instructions.

## Getting Started

### Prerequisites
- C++ compiler
- Standard Template Library (STL)

### Running the Algorithm

1. Clone the repository:
   ```sh
   git clone https://github.com/MuAyman/BlocksRearrangement.git
   cd BlocksRearrangement
   ```
2. Compile the C++ code:
   ```sh
   g++ -o BlocksRearrangement BlocksSearshAlgorithm/BlocksSearshAlgorithm.cpp
   ```

3. Run the executable:\
   ```sh
   ./BlocksRearrangement
   ```
### Example Usage
The `main` function in the `BlocksSearshAlgorithm.cpp` file contains several test cases that demonstrate the algorithm's functionality. Each test case compares the BFS algorithm's solution with a given solution and measures the time taken.

## Code Overview
### State and Block Structures
- `Block`: Represents a single block with a letter and a goal status.
- `State`: A 2D vector representing the arrangement of blocks.

### StateSpace Class
Manages the state, moves, and fixedCount attributes. It includes comparison operators for use with a priority queue.

### Key Functions
- `toString`: Converts a state into a string for easy comparison.
- `IsGoal`: Checks if the current state matches the goal state.
- `getNextStates`: Generates the next possible states from the current state.
- `countMatches`: Updates the state's fixedCount attribute based on goal positions.
- `BFS_Algorithm`: Uses BFS to find the optimal path to the goal state.
- `compare`: Compares the BFS algorithm's solution with a given solution and measures the time taken.

## Contributing
Contributions are welcome! Please fork the repository and submit pull requests.

## License
This project is licensed under the MIT License.

## Contact
For any questions or inquiries, please contact [MuAyman](https://github.com/MuAyman).
