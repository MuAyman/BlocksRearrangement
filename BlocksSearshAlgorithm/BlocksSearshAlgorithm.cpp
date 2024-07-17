#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <sstream>

using namespace std;

using State = vector<vector<char>>;  // 2D vector to store the stacks of blocks
using Moves = vector<pair<string, string>>;  // pair of block and its destination

// Function to convert the state to a string representation for easy comparison
string toString(const State& state) {
    stringstream ss;
    for (const auto& stack : state) {
        for (char block : stack)
            ss << block;
        ss << "/";
    }
    return ss.str();
}

// Function to check if the current state matches the goal state
bool IsGoal(const State& current, const State& goal) {
    int flag = 0;   // to count the num of similar stacks
    for (const auto& curStack : current)
        for (const auto& goalStack : goal)
            if (curStack == goalStack)
                flag++;
    if (flag == goal.size()) return true;   // goal reached if all current stacks are similar to the goal ones
    else return false;
}

// Function to generate the next possible states from the current state
vector<pair<State, Moves>> getNextStates(const State& current, const Moves& path) {
    vector<pair<State, Moves>> nextStates;

    for (int i = 0; i < current.size(); ++i) {
        if (!current[i].empty()) {  // Current stack not empty
            char topBlock = current[i].back();
            for (int j = 0; j < current.size(); ++j) {
                if (i != j) {
                    State newState = current;
                    Moves newPath = path;
                    newPath.push_back(make_pair(string(1, topBlock), current[j].empty() ? "Table" : string(1, current[j].back())));
                    newState[j].push_back(topBlock);
                    newState[i].pop_back();
                    nextStates.push_back(make_pair(newState, newPath));
                }
            }
            // Moving the top block to a new stack (simulating "Table")
            State newState = current;
            Moves newPath = path;
            newState.push_back({ topBlock });
            newPath.push_back(make_pair(string(1, topBlock), "Table"));
            newState[i].pop_back();
            nextStates.push_back(make_pair(newState, newPath));
        }
    }

    return nextStates;
}

// BFS algorithm to find the optimal path to reach the goal state
vector<pair<string, string>> BFS_Algorithm(const State& start, const State& goal) {
    Moves path;
    queue<pair<State, Moves>> BFSq;
    unordered_set<string> visited;

    BFSq.push({ start, {} });
    visited.insert(toString(start));

    while (!BFSq.empty()) {
        State currentState = BFSq.front().first;
        path = BFSq.front().second;
        BFSq.pop();

        if (IsGoal(currentState, goal)) {
            return path;
        }

        vector<pair<State, Moves>> nextStates = getNextStates(currentState, path);

        for (const auto& nextState : nextStates) {
            string stateString = toString(nextState.first);
            if (visited.find(stateString) == visited.end()) {
                BFSq.push(nextState);
                visited.insert(stateString);
            }
        }
    }
    return {}; // return empty path if no path is found
}

int main() {

    State input = { {'C', 'B', 'D'},{'A'} };
    State goal = { {'D', 'C', 'B', 'A'} };

    //State input = { {'D', 'I'},{'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'} };
    //State goal = { {'A', 'C', 'B'},{'G'},{'K'},{'F', 'H', 'I', 'E', 'J', 'D'} };

  /*  State input({ {'D', 'H', 'B', 'E'},{'G', 'A'},{'I', 'C', 'J', 'F'} });
    State goal({ {'D', 'C'},{ 'G', 'E' }, {'F', 'B', 'A', 'H' }, {'J', 'I' } });*/

    //State input = { {'A', 'B', 'C'},{'D', 'E','F'},{'G', 'H', 'I'}};
    //State goal = { {'A', 'B', 'C' ,'D', 'E','F' , 'G', 'H', 'I'} };


    vector<pair<string, string>> path;
    path = BFS_Algorithm(input, goal);

    cout << "The path is: ";
    for (const auto& pathPair : path)
        cout << "(" << pathPair.first << ", " << pathPair.second << ")\n";

    return 0;
}
