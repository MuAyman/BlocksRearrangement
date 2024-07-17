//#include <iostream>
//#include <vector>
//#include <string>
//#include <queue>
//#include <unordered_set>
//#include <sstream>
//#include <algorithm>
//
//using namespace std;
//
//using State = vector<vector<char>>;  // 2D vector to store the stacks of blocks
//using Moves = vector<pair<string, string>>;  // pair of block and its destination
//
//class StateSpace {
//public:
//    State state;
//    Moves move;
//    int fixedCount;  // how close you are to the goal
//    int heuristic;   // heuristic value to guide the search
//
//    StateSpace(State s) : state(s), move({}), fixedCount(0), heuristic(0) {}
//    StateSpace(State s, Moves m, int f, int h) : state(s), move(m), fixedCount(f), heuristic(h) {}
//
//    // for the priority queue comparison
//    bool operator<(const StateSpace& other) const {
//        return (fixedCount + heuristic) > (other.fixedCount + other.heuristic);
//    }
//};
//
//// Function to convert the state to a string representation for easy comparison
//string toString(const State& state) {
//    stringstream ss;
//    for (const auto& stack : state) {
//        for (char block : stack) {
//            ss << block;
//        }
//        ss << "/";
//    }
//    return ss.str();
//}
//
//// Function to check if the current state matches the goal state
//bool IsGoal(const StateSpace& current, const State& goal) {
//    return current.fixedCount == goal.size();
//}
//
//// Heuristic function to estimate the distance to the goal state
//int calculateHeuristic(const State& current, const State& goal) {
//    int heuristic = 0;
//    for (int i = 0; i < goal.size(); ++i) {
//        if (i < current.size()) {
//            for (int j = 0; j < goal[i].size(); ++j) {
//                if (j < current[i].size() && current[i][j] != goal[i][j]) {
//                    ++heuristic;
//                }
//                else if (j >= current[i].size()) {
//                    ++heuristic;
//                }
//            }
//        }
//        else {
//            heuristic += goal[i].size();
//        }
//    }
//    return heuristic;
//}
//
//// Function to generate the next possible states from the current state
//vector<StateSpace> getNextStates(const StateSpace& current, const State& goal) {
//    vector<StateSpace> nextStates;
//
//    for (int i = 0; i < current.state.size(); ++i) {
//        if (!current.state[i].empty()) {
//            char topBlock = current.state[i].back();
//            for (int j = 0; j < current.state.size(); ++j) {
//                if (i != j) {
//                    State newState = current.state;
//                    Moves newPath = current.move;
//                    newPath.push_back(make_pair(string(1, topBlock), current.state[j].empty() ? "Table" : string(1, current.state[j].back())));
//                    newState[j].push_back(topBlock);
//                    newState[i].pop_back();
//                    int newFixedCount = current.fixedCount;
//                    int newHeuristic = calculateHeuristic(newState, goal);
//                    nextStates.push_back({ newState, newPath, newFixedCount, newHeuristic });
//                }
//            }
//        }
//    }
//
//    return nextStates;
//}
//
//// Function to fix all the elements that reach their goal pos and update state's fixedCount attribute
//void fixMatches(StateSpace& current, const State& goal) {
//    current.fixedCount = 0;  // Reset fixedCount
//    for (int i = 0; i < current.state.size(); ++i) {
//        const auto& curStack = current.state[i];
//        const auto& goalStack = goal[i];
//        for (int j = 0; j < min(curStack.size(), goalStack.size()); ++j) {
//            if (curStack[j] == goalStack[j]) {
//                ++current.fixedCount;
//            }
//            else {
//                break;  // Stop at the first mismatch
//            }
//        }
//    }
//}
//
//// BFS algorithm to find the optimal path to reach the goal state
//Moves BFS_Algorithm(const StateSpace& start, const State& goal) {
//    priority_queue<StateSpace> BFSq;
//    unordered_set<string> visited;
//
//    BFSq.push(start);
//    visited.insert(toString(start.state));
//
//    while (!BFSq.empty()) {
//        StateSpace current = BFSq.top();
//        BFSq.pop();
//
//        if (IsGoal(current, goal)) return current.move;
//        fixMatches(current, goal);
//
//        string currentString = toString(current.state);
//        if (visited.find(currentString) == visited.end()) {
//            visited.insert(currentString);
//
//            vector<StateSpace> nextStates = getNextStates(current, goal);
//
//            for (auto& nextState : nextStates) {
//                string stateString = toString(nextState.state);
//                if (visited.find(stateString) == visited.end()) {
//                    fixMatches(nextState, goal);
//                    BFSq.push(nextState);
//                }
//            }
//        }
//    }
//    return {};
//}
//
//int main() {
//    State input = { {'D', 'I'}, {'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'} };
//    State goal = { {'A', 'C', 'B'}, {'G'}, {'K'}, {'F', 'H', 'I', 'E', 'J', 'D'} };
//
//    Moves path;
//    int heuristic = calculateHeuristic(input, goal);
//    StateSpace startState(input, {}, 0, heuristic);
//    path = BFS_Algorithm(startState, goal);
//
//    cout << "The path is: ";
//    for (const auto& pathPair : path)
//        cout << "(" << pathPair.first << ", " << pathPair.second << ")\n";
//
//    return 0;
//}

//
//// Function to generate the next possible states from the current state
//vector<StateSpace> getNextStates(const StateSpace& current, const State& goal) {
//	vector<StateSpace> nextStates;
//	for (int i = 0; i < current.state.size(); ++i) {
//		if (!current.state[i].empty()) {  // current stack not empty
//			if (current.state[i].back() != '-') {  // skipping the marked done
//				char topBlock = current.state[i].back();
//				for (int j = 0; j < current.state.size(); ++j) {
//					if (i != j) {  // here we are moving the block from i to j
//						if (current.state[i].size() == 1 && current.state[j].size() == 0)  // don't move a block from table to table again
//							continue;
//						State newState = current.state;
//						Moves newPath = current.move;
//						newPath.push_back(make_pair(string(1, topBlock), current.state[j].empty() ? "Table" : string(1, current.state[j].back())));
//						newState[j].push_back(topBlock);
//						newState[i].pop_back();
//						//nextStates.push_back({ newState, newPath });
//						int newFixedCount = current.fixedCount;
//						int newHeuristic = calculateHeuristic(newState, goal);
//						nextStates.push_back({ newState, newPath, newFixedCount, newHeuristic });
//					}
//				}
//				// Moving the top block to an existing empty stack
//				bool emptyStackUsed = false;
//				for (int j = 0; j < current.state.size(); ++j) {
//					if (current.state[j].empty()) {  // if you found an empty stack already, use it
//						State newState = current.state;
//						Moves newPath = current.move;
//						newState[j].push_back({ topBlock }); // push back in the empty stack
//						newPath.push_back(make_pair(string(1, topBlock), "Table"));
//						newState[i].pop_back();
//						int newFixedCount = current.fixedCount;
//						int newHeuristic = calculateHeuristic(newState, goal);
//						nextStates.push_back({ newState, newPath, newFixedCount, newHeuristic });
//						emptyStackUsed = true;
//						break;
//					}
//				}
//				// If no empty stack found, create a new stack
//				if (!emptyStackUsed) {
//					State newState = current.state;
//					Moves newPath = current.move;
//					newState.push_back({ topBlock });
//					newPath.push_back(make_pair(string(1, topBlock), "Table"));
//					newState[i].pop_back();
//					int newFixedCount = current.fixedCount;
//					int newHeuristic = calculateHeuristic(newState, goal);
//					nextStates.push_back({ newState, newPath, newFixedCount, newHeuristic });
//				}
//			}
//		}
//	}
//	return nextStates;
//}
//
//// Function to fix all the elements that reach their goal pos and update state's fixedCount attribute
//void fixMatches(StateSpace& current, const StateSpace& goal) {
//	// check if any of the tables (elements on table) are in the right place
//	for (auto& curStack : current.state)	// without const as we are overriding the DONE elements
//		if (!curStack.empty())
//			for (const auto& goalStack : goal.state)
//				if (curStack.front() == goalStack.front())
//				{
//					curStack[0] = '-';	// marking the matched element that it is fixed and not to be moved
//					++current.fixedCount;
//					int counter = 0;	// to iterate over the rest of the stack
//					while (curStack[counter] == '-' && counter < curStack.size() - 1)	// extend the checking to the elements above
//					{
//						++counter;
//						if (curStack[counter] == goalStack[counter])
//						{
//							curStack[counter] = '-';	// mark done
//							++current.fixedCount;
//						}
//					}
//				}
//}
