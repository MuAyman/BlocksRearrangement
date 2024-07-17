//#include <iostream>
//#include <vector>
//#include <string>
//#include <queue>
//#include <unordered_set>
//#include <algorithm>
//
//using namespace std;
//
//
///* ===================================== */
///*   ============ TO DO =============
//* Add the functions to the class
//
//
//
//										 */
//										 /* ===================================== */
//
//
//using State = vector<vector<char>>;  // 2D vector to store the stacks of blocks
//using Moves = vector<pair<string, string>>;  // pair of block and its destination
//
//class StateSpace {	// we can of course make getters and setters but 
//public:				// not worth it in the context of a test task
//	State state;
//	Moves move;
//	int fixedCount;	// how close you are to the goal
//	int heuristic;   // heuristic value to guide the search
//
//	StateSpace(State s) : state(s), move({}), fixedCount(0) {}
//	StateSpace(State s, Moves m) : state(s), move(m), fixedCount(0), heuristic(0) {}
//	StateSpace(State s, Moves m, int f, int h) : state(s), move(m), fixedCount(f), heuristic(h) {}
//
//	// for the priority queue comparison
//	bool operator<(const StateSpace& other) const {
//		return (fixedCount + heuristic) < (other.fixedCount + other.heuristic);
//	}
//};
//
//
//// Function to convert the state to a string representation for easy comparison
//string toString(const State& state) {
//	string wholeState;
//	for (const auto& stack : state) {  // loop on the stacks, avoid copying the stack + not changing it
//		for (char block : stack)  // loop on the blocks
//			wholeState += block;
//		wholeState += "/";  // separate the stacks from each other
//	}
//	return wholeState;
//}
//
//// Function to check if the current state matches the goal state
//bool IsGoal(const StateSpace& current) {
//	int totalElementsNum = 0;
//	for (const auto& stack : current.state)
//		totalElementsNum += stack.size();
//	if (totalElementsNum == current.fixedCount) return true;
//	else return false;
//}
//
//// Heuristic function to estimate the distance to the goal state
//int calculateHeuristic(const State& current, const State& goal) {
//	int heuristic = 0;
//	for (int i = 0; i < goal.size(); ++i) {
//		if (i < current.size()) {
//			for (int j = 0; j < goal[i].size(); ++j) {
//				if (j < current[i].size() && current[i][j] != goal[i][j]) {
//					++heuristic;
//				}
//				else if (j >= current[i].size()) {
//					++heuristic;
//				}
//			}
//		}
//		else {
//			heuristic += goal[i].size();
//		}
//	}
//	return heuristic;
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
//
//// BFS algorithm to find the optimal path to reach the goal state
//Moves BFS_Algorithm(const StateSpace& start, const StateSpace& goal) {
//	//Moves path;
//	priority_queue<StateSpace> BFSq;
//	unordered_set<string> visited;
//
//	BFSq.push(start);
//	//visited.insert(toString(start.state));
//	int fixedCount = 0;	// for the priority queue
//	//static int maxFixedCount = 0;	// to store the nearest to goal we reached and disregard the others
//	int count = 0, count2 = 0;
//	while (!BFSq.empty()) {
//		StateSpace current = BFSq.top();
//		//path = BFSq.top().move;
//		BFSq.pop();
//
//		if (IsGoal(current))	return current.move;
//		fixMatches(current, goal);
//
//		++count; count2 = 0;
//		cout << "\n visited " << count << " : " << toString(current.state);
//		for (auto stack : current.state)
//			for (auto block : stack)
//				(block == '-') ? ++count2 : count2;
//		cout << "\t\t fixed: " << count2;
//
//
//		string currentString = toString(current.state);
//		if (visited.find(currentString) == visited.end()) {
//			visited.insert(currentString);
//
//			vector<StateSpace> nextStates = getNextStates(current, goal.state);
//
//			for (auto& nextState : nextStates) {
//				string stateString = toString(nextState.state);
//				if (visited.find(stateString) == visited.end()) {
//					fixMatches(nextState, goal);
//					BFSq.push(nextState);
//				}
//			}
//		}
//	}
//	return {}; // return empty path if no path is found
//}
//
//int main() {
//
//	//State input = { {'C', 'B', 'D'},{'A'} };
//	//State goal = { {'D', 'C', 'B', 'A'} };
//
//	//State input = { {'D', 'I'},{'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'} };
//	//State goal = { {'A', 'C', 'B'},{'G'},{'K'},{'F', 'H', 'I', 'E', 'J', 'D'} };
//
//	State input({ {'D', 'H', 'B', 'E'},{'G', 'A'},{'I', 'C', 'J', 'F'} });
//	State goal({ {'D', 'C'},{ 'G', 'E' }, {'F', 'B', 'A', 'H' }, {'J', 'I' } });
//
//	//State input = { {'A', 'B', 'C'},{'D', 'E','F'},{'G', 'H', 'I'}};
//	//State goal = { {'A', 'B', 'C' ,'D', 'E','F' , 'G', 'H', 'I'} };
//
//	Moves path;
//	int heuristic = calculateHeuristic(input, goal);
//	StateSpace startState(input, {}, 0, heuristic);
//	path = BFS_Algorithm(startState, goal);
//
//
//	cout << "The path is: ";
//	for (const auto& pathPair : path)
//		cout << "(" << pathPair.first << ", " << pathPair.second << ")\n";
//
//	return 0;
//}
