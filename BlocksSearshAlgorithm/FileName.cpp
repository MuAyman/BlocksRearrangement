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
vector<StateSpace> getNextStates(const StateSpace &current, const State &goal)
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

          vector<StateSpace> nextStates = getNextStates(current, goal);

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

void compare(State in, State goal, State sol)
{
     int n = 1;
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
     compare(input1, goal1, sol1);

     State input2 = {{'D', 'H', 'B', 'E'}, {'G', 'A'}, {'I', 'C', 'J', 'F'}};
     State goal2 = {{'D', 'C'}, {'G', 'E'}, {'F', 'B', 'A', 'H'}, {'J', 'I'}};
     State sol2 = {{'A', '-'}, {'E', 'G'}, {'F', '-'}, {'B', 'F'}, {'A', 'B'}, {'H', 'A'}, {'J', '-'}, {'C', 'D'}, {'I', 'J'}};
     compare(input2, goal2, sol2);

     State input3 = {{'E', 'N', 'H', 'O'}, {'L', 'F', 'K', 'B'}, {'M', 'C'}, {'A', 'I', 'J'}, {'G', 'D'}};
     State goal3 = {{'O', 'A', 'K', 'D', 'J', 'N'}, {'I'}, {'F', 'H', 'C'}, {'E', 'L', 'M'}, {'G', 'B'}};
     State sol3 = {{'J', '-'}, {'I', '-'}, {'O', '-'}, {'A', 'O'}, {'B', '-'}, {'K', 'A'}, {'D', 'K'}, {'B', 'G'}, {'J', 'D'}, {'H', '-'}, {'N', 'J'}, {'F', '-'}, {'L', 'E'}, {'H', 'F'}, {'C', 'H'}, {'M', 'L'}};
     compare(input3, goal3, sol3);

     State input4 = {{'D', 'I'}, {'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'}};
     State goal4 = {{'A', 'C', 'B'}, {'G'}, {'K'}, {'F', 'H', 'I', 'E', 'J', 'D'}};
     State sol4 = {{'G', '-'}, {'I', '-'}, {'C', 'D'}, {'B', 'C'}, {'H', 'F'}, {'I', 'H'}, {'A', '-'}, {'E', 'I'}, {'K', '-'}, {'J', 'E'}, {'B', '-'}, {'C', 'A'}, {'D', 'J'}, {'B', 'C'}};
     compare(input4, goal4, sol4);

     State input5 = {{'D', 'G', 'C', 'P', 'J', 'O', 'I', 'H'}, {'A', 'Q', 'M', 'K'}, {'L', 'F'}, {'B', 'N', 'E'}};
     State goal5 = {{'H', 'B'}, {'E', 'P', 'D', 'K'}, {'O', 'Q', 'G'}, {'N', 'M', 'C'}, {'F', 'I', 'J'}, {'L', 'A'}};
     State sol5 = {{'H', '-'}, {'F', '-'}, {'I', 'F'}, {'O', '-'}, {'J', 'I'}, {'E', '-'}, {'P', 'E'}, {'N', '-'}, {'B', 'H'}, {'K', '-'}, {'M', 'N'}, {'C', 'M'}, {'G', 'Q'}, {'D', 'P'}, {'K', 'D'}, {'G', '-'}, {'Q', 'O'}, {'A', 'L'}, {'G', 'Q'}};
     compare(input5, goal5, sol5);

     State input6 = {{'F', 'B', 'C'}, {'H', 'I'}, {'M', 'E', 'J'}, {'L', 'D', 'A', 'G', 'K'}};
     State goal6 = {{'D', 'H', 'J', 'E', 'C'}, {'L'}, {'G', 'B', 'M'}, {'I', 'A', 'F'}, {'K'}};
     State sol6 = {{'K', '-'}, {'C', '-'}, {'G', '-'}, {'B', 'G'}, {'A', '-'}, {'D', '-'}, {'I', '-'}, {'H', 'D'}, {'J', 'H'}, {'E', 'J'}, {'M', 'B'}, {'C', 'E'}, {'A', 'I'}, {'F', 'A'}};
     compare(input6, goal6, sol6);

     State input7 = {{'J', 'B', 'A', 'E', 'O', 'N'}, {'I', 'G', 'K', 'P', 'Q', 'D', 'L', 'H'}, {'F'}, {'M', 'C'}};
     State goal7 = {{'O', 'E', 'H', 'L'}, {'A'}, {'M', 'I', 'F'}, {'B', 'K', 'G', 'N'}, {'P'}, {'J', 'Q', 'D'}, {'C'}};
     State sol7 = {{'C', '-'}, {'N', '-'}, {'O', '-'}, {'E', 'O'}, {'A', '-'}, {'H', 'E'}, {'L', 'H'}, {'B', '-'}, {'D', '-'}, {'Q', 'J'}, {'D', 'Q'}, {'P', '-'}, {'K', 'B'}, {'G', 'K'}, {'N', 'G'}, {'I', 'M'}, {'F', 'I'}};
     compare(input7, goal7, sol7);

     State input8 = {{'O', 'B', 'D'}, {'F', 'Q', 'I', 'P', 'A'}, {'C', 'E', 'M', 'G'}, {'R', 'N', 'L'}, {'H', 'J'}, {'K'}};
     State goal8 = {{'M', 'G', 'I', 'O'}, {'J', 'L', 'P'}, {'H', 'Q', 'D', 'E', 'C', 'B'}, {'N', 'K'}, {'R', 'A', 'F'}};
     State sol8 = {{'J', '-'}, {'L', 'J'}, {'A', '-'}, {'P', 'L'}, {'I', '-'}, {'Q', 'H'}, {'D', 'Q'}, {'N', '-'}, {'K', 'N'}, {'A', 'R'}, {'F', 'A'}, {'G', '-'}, {'M', '-'}, {'E', 'D'}, {'C', 'E'}, {'B', 'C'}, {'G', 'M'}, {'I', 'G'}, {'O', 'I'}};
     compare(input8, goal8, sol8);

     State input9 = {{'C', 'B', 'F', 'O', 'D'}, {'I', 'L', 'H', 'G', 'P', 'K', 'N'}, {'J', 'E'}, {'A', 'M'}};
     State goal9 = {{'A', 'K'}, {'C', 'G', 'F', 'O'}, {'D', 'H', 'J'}, {'B', 'I'}, {'M', 'P'}, {'E', 'L'}, {'N'}};
     State sol9 = {{'N', '-'}, {'M', '-'}, {'K', 'A'}, {'P', 'M'}, {'D', '-'}, {'G', '-'}, {'H', 'D'}, {'E', '-'}, {'L', 'E'}, {'J', 'H'}, {'O', '-'}, {'F', '-'}, {'O', 'F'}, {'B', '-'}, {'I', 'B'}, {'G', 'C'}, {'O', '-'}, {'F', 'G'}, {'O', 'F'}};
     compare(input9, goal9, sol9);

     State input10 = {{'J', 'H', 'I', 'D', 'O', 'P', 'B', 'F', 'G', 'C', 'E', 'K', 'A'}, {'Q', 'L'}, {'N', 'M'}};
     State goal10 = {{'P', 'N', 'H', 'C'}, {'O', 'K', 'Q', 'J', 'D', 'B', 'I'}, {'M', 'E', 'A'}, {'G', 'L'}, {'F'}};
     State sol10 = {{'A', '-'}, {'K', '-'}, {'M', '-'}, {'E', 'M'}, {'A', 'E'}, {'C', '-'}, {'G', '-'}, {'L', 'G'}, {'F', '-'}, {'B', '-'}, {'P', '-'}, {'N', 'P'}, {'O', '-'}, {'K', 'O'}, {'Q', 'K'}, {'D', '-'}, {'I', '-'}, {'H', 'N'}, {'C', 'H'}, {'J', 'Q'}, {'D', 'J'}, {'B', 'D'}, {'I', 'B'}};
     compare(input10, goal10, sol10);

     State input11 = {{'A', 'B', 'C'}, {'D', 'E'}};
     State goal11 = {{'D', 'E', 'A', 'B', 'C'}};
     State sol11 = {{'C', '-'}, {'B', '-'}, {'A', 'E'}, {'B', 'A'}, {'C', 'B'}};
     compare(input11, goal11, sol11);

     State input12 = {{'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'}};
     State goal12 = {{'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'}};
     State sol12 = {{'I', '-'}, {'H', 'I'}, {'G', 'H'}, {'F', 'G'}, {'E', 'F'}, {'D', 'E'}, {'C', 'D'}, {'B', 'C'}, {'A', 'B'}};
     compare(input12, goal12, sol12);

     State input13 = {{'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'}};
     State goal13 = {{'F', 'D', 'C', 'I', 'G', 'A'}, {'B', 'E', 'H'}};
     State sol13 = {{'F', '-'}, {'E', '-'}, {'D', 'F'}, {'C', 'D'}, {'I', 'C'}, {'B', '-'}, {'E', 'B'}, {'H', '-'}, {'H', 'E'}, {'G', 'I'}, {'A', 'G'}};
     compare(input13, goal13, sol13);
     // Add more tests as needed
     return 0;
}
