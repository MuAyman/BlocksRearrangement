#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <sstream>

using namespace std;

using State = vector<vector<char>>;  // 2D vector to store the stacks of blocks
using Moves = vector<pair<string, string>>;  // pair of block and its destination


class StateSpace {	// we can of course make getters and setters but 
public:				// not worth it in the context of a test task
    State state;
    Moves move;
    int fixedCount;	// how close you are to the goal
    int heuristic;   // heuristic value to guide the search

    StateSpace(State s) : state(s), move({}), fixedCount(0) {}
    StateSpace(State s, Moves m) : state(s), move(m), fixedCount(0), heuristic(0) {}
    StateSpace(State s, Moves m, int f, int h) : state(s), move(m), fixedCount(f), heuristic(h) {}

    // for the priority queue comparison
    bool operator<(const StateSpace& other) const {
        return (fixedCount + heuristic) < (other.fixedCount + other.heuristic);
    }
};

// Function to convert the state to a string representation for easy comparison
string toString(const State& state) {
    string wholeState;
    for (const auto& stack : state) {  // loop on the stacks, avoid copying the stack + not changing it
        for (char block : stack)  // loop on the blocks
            wholeState += block;
        wholeState += "/";  // separate the stacks from each other
    }
    return wholeState;
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
            //State newState = current;
            //Moves newPath = path;
            //newState.push_back({ topBlock });
            //newPath.push_back(make_pair(string(1, topBlock), "Table"));
            //newState[i].pop_back();
            //nextStates.push_back(make_pair(newState, newPath));
            // Moving the top block to an existing empty stack
            bool emptyStackUsed = false;
            for (int j = 0; j < current.size(); ++j) {
                if (current[j].empty()) {  // if you found an empty stack already, use it
                    State newState = current;
                    Moves newPath = path;
                    newState[j].push_back({ topBlock }); // push back in the empty stack
                    newPath.push_back(make_pair(string(1, topBlock), "Table"));
                    newState[i].pop_back();
                    nextStates.push_back({ newState, newPath });
                    emptyStackUsed = true;
                    break;
                }
            }
            // If no empty stack found, create a new stack
            if (!emptyStackUsed) {
                State newState = current;
                Moves newPath = path;
                newState.push_back({ topBlock });
                newPath.push_back(make_pair(string(1, topBlock), "Table"));
                newState[i].pop_back();
                nextStates.push_back({ newState, newPath });
            }
        }
    }

    return nextStates;
}

// Function to fix all the elements that reach their goal pos and update state's fixedCount attribute
void fixMatches(State& current, const State& goal) {
    // check if any of the tables (elements on table) are in the right place
    for (auto& curStack : current)	// without const as we are overriding the DONE elements
        if (!curStack.empty())
            for (const auto& goalStack : goal)
                if (curStack.front() == goalStack.front())
                {
                    curStack[0] = '-';	// marking the matched element that it is fixed and not to be moved
                    //++current.fixedCount;
                    int counter = 0;	// to iterate over the rest of the stack
                    while (curStack[counter] == '-' && counter < curStack.size() - 1)	// extend the checking to the elements above
                    {
                        ++counter;
                        if (curStack[counter] == goalStack[counter])
                        {
                            curStack[counter] = '-';	// mark done
                            //++current.fixedCount;
                        }
                    }
                }
}

// BFS algorithm to find the optimal path to reach the goal state
vector<pair<string, string>> BFS_Algorithm(const State& start, const State& goal) {
    Moves path;
    queue<pair<State, Moves>> BFSq;
    unordered_set<string> visited;

    BFSq.push({ start, {} });
    visited.insert(toString(start));

    int count = 0, count2 = 0;
    while (!BFSq.empty()) {
        State currentState = BFSq.front().first;
        path = BFSq.front().second;
        BFSq.pop();

        if (IsGoal(currentState, goal))   return path;
        //fixMatches(currentState, goal);

        ++count; count2 = 0;
        cout << "\n visited " << count << " : " << toString(currentState);
        for (auto stack : currentState)
            for (auto block : stack)
                (block == '-') ? ++count2 : count2;
        cout << "\t\t fixed: " << count2;

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

    State input = { {'P','D', 'A', 'C','B'}};
    State goal = { {'D', 'C', 'B'},{'A', 'P'} };

    //State input = { {'D', 'I'},{'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'} };
    //State goal = { {'A', 'C', 'B'},{'G'},{'K'},{'F', 'H', 'I', 'E', 'J', 'D'} };

  /*  State input({ {'D', 'H', 'B', 'E'},{'G', 'A'},{'I', 'C', 'J', 'F'} });
    State goal({ {'D', 'C'},{ 'G', 'E' }, {'F', 'B', 'A', 'H' }, {'J', 'I' } });*/

    //State input = { {'A', 'B', 'C'},{'D', 'E','F'},{'G', 'H', 'I'}};
    //State goal = { {'A', 'B', 'C' ,'D', 'E','F' , 'G', 'H', 'I'} };


    vector<pair<string, string>> path;
    path = BFS_Algorithm(input, goal);

    cout << "\n\nThe path is: ";
    for (const auto& pathPair : path)
        cout << "(" << pathPair.first << ", " << pathPair.second << ")\n";

    return 0;
}
