#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Block
{
     char letter;
     bool isgoal;

     Block(char let) : letter(let), isgoal(false) {}
};

using State = vector<vector<Block>>;        // 2D vector to store the stacks of blocks
using Moves = vector<pair<string, string>>; // pair of block and its destination

class StateSpace
{
public:
     State state;
     Moves move;
     int heuristic; // heuristic value

     StateSpace() : state({}), move({}), heuristic(0) {}
     StateSpace(State s) : state(s), move({}), heuristic(0) {}
     StateSpace(State s, Moves m) : state(s), move(m), heuristic(0) {}
     StateSpace(State s, Moves m, int h) : state(s), move(m), heuristic(h) {}

     bool operator<(const StateSpace &other) const
     {
          return (heuristic) > (other.heuristic); // Min-heap based on heuristic value
     }
};

// Function to convert the state to a string representation for easy comparison
string toString(const State &state)
{
     string wholeState;
     for (const auto &stack : state)
     {
          for (auto block : stack)
               wholeState += block.letter;
          wholeState += "/"; // separate the stacks from each other
     }
     return wholeState;
}

// Function to check if the current state matches the goal state
bool IsGoal(const State &current, const State &goal)
{
     for (int i = 0; i < current.size(); ++i)
     {
          if (current[i].size() != goal[i].size())
               return false;
          for (int j = 0; j < current[i].size(); ++j)
          {
               if (current[i][j].letter != goal[i][j].letter)
                    return false;
          }
     }
     return true;
}

// Function to calculate a heuristic value for the current state
int calculateHeuristic(const State &current, const State &goal)
{
     int h = 0;
     for (int i = 0; i < current.size(); ++i)
     {
          for (int j = 0; j < current[i].size(); ++j)
          {
               char curBlock = current[i][j].letter;
               bool found = false;
               for (int k = 0; k < goal.size(); ++k)
               {
                    for (int l = 0; l < goal[k].size(); ++l)
                    {
                         if (curBlock == goal[k][l].letter)
                         {
                              h += abs(i - k) + abs(j - l); // Manhattan distance as heuristic
                              found = true;
                              break;
                         }
                    }
                    if (found)
                         break;
               }
          }
     }
     return h;
}

// function to generate the next possible states from the current state
vector<StateSpace> getNextStates(const StateSpace &current)
{
     vector<StateSpace> nextStates;

     for (int i = 0; i < current.state.size(); ++i)
     {
          if (!current.state[i].empty())
          {
               Block topBlock = current.state[i].back();

               for (int j = 0; j < current.state.size(); ++j)
               {
                    if (i != j)
                    {
                         State newState = current.state;
                         Moves newPath = current.move;
                         newPath.push_back(make_pair(string(1, topBlock.letter), current.state[j].empty() ? "Table" : string(1, current.state[j].back().letter)));
                         newState[j].push_back(topBlock);
                         newState[i].pop_back();
                         int newHeuristic = calculateHeuristic(newState, goal); // Calculate new heuristic
                         nextStates.push_back({newState, newPath, newHeuristic});
                    }
               }
          }
     }
     return nextStates;
}

// BFS algorithm to find the optimal path to reach the goal state
Moves BFS_Algorithm(const State &start, const State &goal)
{
     priority_queue<StateSpace> BFSq;
     unordered_set<string> visited;

     StateSpace initialState(start);
     initialState.heuristic = calculateHeuristic(start, goal);
     BFSq.push(initialState);
     visited.insert(toString(start));

     while (!BFSq.empty())
     {
          StateSpace current = BFSq.top();
          BFSq.pop();

          if (IsGoal(current.state, goal))
               return current.move;

          vector<StateSpace> nextStates = getNextStates(current);

          for (auto &nextState : nextStates)
          {
               string stateString = toString(nextState.state);
               if (visited.find(stateString) == visited.end())
               {
                    visited.insert(stateString);
                    BFSq.push(nextState);
               }
          }
     }
     return {};
}

void compare(State in, State goal, State sol, int n)
{
     Moves path = BFS_Algorithm(in, goal);

     cout << "\n==================================";
     cout << "\nTest num: " << n;
     cout << "\n\tmy sol moves: " << path.size() << "\n\t  Test moves: " << sol.size() << "\n";
     // Uncomment to see the moves
     // cout << "The moves: ";
     // for (const auto& pair : path)
     // 	cout << "(" << pair.first << ", " << pair.second << "), ";
     cout << "\n";
}

int main()
{
     State input1 = {{'A', 'B', 'C'}, {'D', 'E'}};
     State goal1 = {{'A', 'C'}, {'D', 'E', 'B'}};
     State sol1 = {{'C', '-'}, {'B', 'E'}, {'C', 'A'}};
     compare(input1, goal1, sol1, 1);

     // Add more tests as needed
     return 0;
}
