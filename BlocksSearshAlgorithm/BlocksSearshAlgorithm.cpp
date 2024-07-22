#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <sstream>

using namespace std;

using State = vector<vector<char>>;	    // 2D vector to store the stacks of blocks
using Moves = vector<pair<string, string>>; // pair of block and its destination

class StateSpace
{	   // we can of course make getters and setters but
public: // not worth it in the context of a test task
	State state;
	Moves move;
	int fixedCount; // how close you are to the goal
	int heuristic;	 // heuristic value to guide the search

	StateSpace(State s) : state(s), move({}), fixedCount(0) {}
	StateSpace(State s, Moves m) : state(s), move(m), fixedCount(0), heuristic(0) {}
	StateSpace(State s, Moves m, int f, int h) : state(s), move(m), fixedCount(f), heuristic(h) {}

	// for the priority queue comparison
	bool operator<(const StateSpace &other) const
	{
		return (fixedCount + heuristic) < (other.fixedCount + other.heuristic);
	}
};

// Function to convert the state to a string representation for easy comparison
string toString(const State &state)
{
	State s = state; // to store sorted state
	bool swapped = 0;
	// bubble sort for the stacks of state (to insure AD/BC is same as BC/AD)
	for (int i = 0; i < s.size() - 1; ++i)
	{
		swapped = 0;
		for (int j = 0; j < s.size() - i - 1; ++j)
		{
			if (s[j][0] > s[j + 1][0]) // comparing the tables (elements on the table) and sorting accourrding to them
			{
				swap(s[j], s[j + 1]); // swaping the two stacks of the state vector
				swapped = 1;
			}
		}
		if (!swapped)
			break; // already sorted
	}

	// now to convert the sorted state vector so now AB/BC is same as BC/AD (avoid reapting states)
	string wholeState;
	for (const auto &stack : s) // avoid copying stacks + not changing it
	{
		for (auto blocks : stack)
			wholeState += blocks;
		wholeState += "/"; // separate stacks from each other
	}
	return wholeState;
}

// Function to check if the current state matches the goal state
bool IsGoal(const State &current, const State &goal)
{
	int flag = 0; // to count the num of similar stacks
	for (const auto &curStack : current)
		for (const auto &goalStack : goal)
			if (curStack == goalStack)
				flag++;
	if (flag == goal.size())
		return true; // goal reached if all current stacks are similar to the goal ones
	else
		return false;
}

// Function to generate the next possible states from the current state
vector<pair<State, Moves>> getNextStates(const State &current, const Moves &path)
{
	vector<pair<State, Moves>> nextStates;

	for (int i = 0; i < current.size(); ++i)
	{
		if (!current[i].empty())
		{ // Current stack not empty
			char topBlock = current[i].back();
			for (int j = 0; j < current.size(); ++j)
			{
				if (i != j)
				{
					State newState = current;
					Moves newPath = path;
					newPath.push_back(make_pair(string(1, topBlock), current[j].empty() ? "Table" : string(1, current[j].back())));
					newState[j].push_back(topBlock);
					newState[i].pop_back();
					nextStates.push_back(make_pair(newState, newPath));
				}
			}
			// Moving the top block to a new stack (simulating "Table")
			// State newState = current;
			// Moves newPath = path;
			// newState.push_back({ topBlock });
			// newPath.push_back(make_pair(string(1, topBlock), "Table"));
			// newState[i].pop_back();
			// nextStates.push_back(make_pair(newState, newPath));
			// Moving the top block to an existing empty stack
			bool emptyStackUsed = false;
			for (int j = 0; j < current.size(); ++j)
			{
				if (current[j].empty())
				{ // if you found an empty stack already, use it
					State newState = current;
					Moves newPath = path;
					newState[j].push_back({topBlock}); // push back in the empty stack
					newPath.push_back(make_pair(string(1, topBlock), "Table"));
					newState[i].pop_back();
					nextStates.push_back({newState, newPath});
					emptyStackUsed = true;
					break;
				}
			}
			// If no empty stack found, create a new stack
			if (!emptyStackUsed)
			{
				State newState = current;
				Moves newPath = path;
				newState.push_back({topBlock});
				newPath.push_back(make_pair(string(1, topBlock), "Table"));
				newState[i].pop_back();
				nextStates.push_back({newState, newPath});
			}
		}
	}

	return nextStates;
}

// Function to fix all the elements that reach their goal pos and update state's fixedCount attribute
void fixMatches(State &current, const State &goal)
{
	// check if any of the tables (elements on table) are in the right place
	for (auto &curStack : current) // without const as we are overriding the DONE elements
		if (!curStack.empty())
			for (const auto &goalStack : goal)
				if (curStack.front() == goalStack.front())
				{
					curStack[0] = '-'; // marking the matched element that it is fixed and not to be moved
					//++current.fixedCount;
					int counter = 0;										 // to iterate over the rest of the stack
					while (curStack[counter] == '-' && counter < curStack.size() - 1) // extend the checking to the elements above
					{
						++counter;
						if (curStack[counter] == goalStack[counter])
						{
							curStack[counter] = '-'; // mark done
												//++current.fixedCount;
						}
					}
				}
}

// BFS algorithm to find the optimal path to reach the goal state
vector<pair<string, string>> BFS_Algorithm(const State &start, const State &goal)
{
	Moves path;
	queue<pair<State, Moves>> BFSq;
	unordered_set<string> visited;

	BFSq.push({start, {}});
	visited.insert(toString(start));

	int count = 0, count2 = 0;
	while (!BFSq.empty())
	{
		State currentState = BFSq.front().first;
		path = BFSq.front().second;
		BFSq.pop();

		if (IsGoal(currentState, goal))
			return path;
		// fixMatches(currentState, goal);

		//++count; count2 = 0;
		// cout << "\n visited " << count << " : " << toString(currentState);
		// for (auto stack : currentState)
		//    for (auto block : stack)
		//        (block == '-') ? ++count2 : count2;
		// cout << "\t\t fixed: " << count2;

		vector<pair<State, Moves>> nextStates = getNextStates(currentState, path);

		for (const auto &nextState : nextStates)
		{
			string stateString = toString(nextState.first);
			if (visited.find(stateString) == visited.end())
			{
				BFSq.push(nextState);
				visited.insert(stateString);
			}
		}
	}
	return {}; // return empty path if no path is found
}

void compare(State in, State goal, State sol, int n)
{

	vector<pair<string, string>> path = BFS_Algorithm(in, goal);

	cout << "\n"
		<< "Test " << n << "\t\tmy sol moves: " << path.size() << "\n\t\t  Test moves: " << sol.size() << "\n";
}

int main()
{

	State input = {{'P', 'D', 'A', 'C', 'B'}};
	State goal = {{'D', 'C', 'B'}, {'A', 'P'}};

	State input1 = {{'A', 'B', 'C'}, {'D', 'E'}};
	State goal1 = {{'A', 'C'}, {'D', 'E', 'B'}};
	State sol1 = {{'C', '-'}, {'B', 'E'}, {'C', 'A'}};
	// compare(input1, goal1, sol1);

	State input2 = {{'D', 'H', 'B', 'E'}, {'G', 'A'}, {'I', 'C', 'J', 'F'}};
	State goal2 = {{'D', 'C'}, {'G', 'E'}, {'F', 'B', 'A', 'H'}, {'J', 'I'}};
	State sol2 = {{'A', '-'}, {'E', 'G'}, {'F', '-'}, {'B', 'F'}, {'A', 'B'}, {'H', 'A'}, {'J', '-'}, {'C', 'D'}, {'I', 'J'}};
	// compare(input2, goal2, sol2);

	State input3 = {{'E', 'N', 'H', 'O'}, {'L', 'F', 'K', 'B'}, {'M', 'C'}, {'A', 'I', 'J'}, {'G', 'D'}};
	State goal3 = {{'O', 'A', 'K', 'D', 'J', 'N'}, {'I'}, {'F', 'H', 'C'}, {'E', 'L', 'M'}, {'G', 'B'}};
	State sol3 = {{'J', '-'}, {'I', '-'}, {'O', '-'}, {'A', 'O'}, {'B', '-'}, {'K', 'A'}, {'D', 'K'}, {'B', 'G'}, {'J', 'D'}, {'H', '-'}, {'N', 'J'}, {'F', '-'}, {'L', 'E'}, {'H', 'F'}, {'C', 'H'}, {'M', 'L'}};
	// compare(input3, goal3, sol3);

	State input4 = {{'D', 'I'}, {'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'}};
	State goal4 = {{'A', 'C', 'B'}, {'G'}, {'K'}, {'F', 'H', 'I', 'E', 'J', 'D'}};
	State sol4 = {{'G', '-'}, {'I', '-'}, {'C', 'D'}, {'B', 'C'}, {'H', 'F'}, {'I', 'H'}, {'A', '-'}, {'E', 'I'}, {'K', '-'}, {'J', 'E'}, {'B', '-'}, {'C', 'A'}, {'D', 'J'}, {'B', 'C'}};
	// compare(input4, goal4, sol4);

	State input5 = {{'D', 'G', 'C', 'P', 'J', 'O', 'I', 'H'}, {'A', 'Q', 'M', 'K'}, {'L', 'F'}, {'B', 'N', 'E'}};
	State goal5 = {{'H', 'B'}, {'E', 'P', 'D', 'K'}, {'O', 'Q', 'G'}, {'N', 'M', 'C'}, {'F', 'I', 'J'}, {'L', 'A'}};
	State sol5 = {{'H', '-'}, {'F', '-'}, {'I', 'F'}, {'O', '-'}, {'J', 'I'}, {'E', '-'}, {'P', 'E'}, {'N', '-'}, {'B', 'H'}, {'K', '-'}, {'M', 'N'}, {'C', 'M'}, {'G', 'Q'}, {'D', 'P'}, {'K', 'D'}, {'G', '-'}, {'Q', 'O'}, {'A', 'L'}, {'G', 'Q'}};
	// compare(input5, goal5, sol5);

	State input6 = {{'F', 'B', 'C'}, {'H', 'I'}, {'M', 'E', 'J'}, {'L', 'D', 'A', 'G', 'K'}};
	State goal6 = {{'D', 'H', 'J', 'E', 'C'}, {'L'}, {'G', 'B', 'M'}, {'I', 'A', 'F'}, {'K'}};
	State sol6 = {{'K', '-'}, {'C', '-'}, {'G', '-'}, {'B', 'G'}, {'A', '-'}, {'D', '-'}, {'I', '-'}, {'H', 'D'}, {'J', 'H'}, {'E', 'J'}, {'M', 'B'}, {'C', 'E'}, {'A', 'I'}, {'F', 'A'}};
	// compare(input6, goal6, sol6);

	State input7 = {{'J', 'B', 'A', 'E', 'O', 'N'}, {'I', 'G', 'K', 'P', 'Q', 'D', 'L', 'H'}, {'F'}, {'M', 'C'}};
	State goal7 = {{'O', 'E', 'H', 'L'}, {'A'}, {'M', 'I', 'F'}, {'B', 'K', 'G', 'N'}, {'P'}, {'J', 'Q', 'D'}, {'C'}};
	State sol7 = {{'C', '-'}, {'N', '-'}, {'O', '-'}, {'E', 'O'}, {'A', '-'}, {'H', 'E'}, {'L', 'H'}, {'B', '-'}, {'D', '-'}, {'Q', 'J'}, {'D', 'Q'}, {'P', '-'}, {'K', 'B'}, {'G', 'K'}, {'N', 'G'}, {'I', 'M'}, {'F', 'I'}};
	// compare(input7, goal7, sol7);

	State input8 = {{'O', 'B', 'D'}, {'F', 'Q', 'I', 'P', 'A'}, {'C', 'E', 'M', 'G'}, {'R', 'N', 'L'}, {'H', 'J'}, {'K'}};
	State goal8 = {{'M', 'G', 'I', 'O'}, {'J', 'L', 'P'}, {'H', 'Q', 'D', 'E', 'C', 'B'}, {'N', 'K'}, {'R', 'A', 'F'}};
	State sol8 = {{'J', '-'}, {'L', 'J'}, {'A', '-'}, {'P', 'L'}, {'I', '-'}, {'Q', 'H'}, {'D', 'Q'}, {'N', '-'}, {'K', 'N'}, {'A', 'R'}, {'F', 'A'}, {'G', '-'}, {'M', '-'}, {'E', 'D'}, {'C', 'E'}, {'B', 'C'}, {'G', 'M'}, {'I', 'G'}, {'O', 'I'}};
	// compare(input8, goal8, sol8);

	State input9 = {{'C', 'B', 'F', 'O', 'D'}, {'I', 'L', 'H', 'G', 'P', 'K', 'N'}, {'J', 'E'}, {'A', 'M'}};
	State goal9 = {{'A', 'K'}, {'C', 'G', 'F', 'O'}, {'D', 'H', 'J'}, {'B', 'I'}, {'M', 'P'}, {'E', 'L'}, {'N'}};
	State sol9 = {{'N', '-'}, {'M', '-'}, {'K', 'A'}, {'P', 'M'}, {'D', '-'}, {'G', '-'}, {'H', 'D'}, {'E', '-'}, {'L', 'E'}, {'J', 'H'}, {'O', '-'}, {'F', '-'}, {'O', 'F'}, {'B', '-'}, {'I', 'B'}, {'G', 'C'}, {'O', '-'}, {'F', 'G'}, {'O', 'F'}};
	// compare(input9, goal9, sol9);

	State input10 = {{'J', 'H', 'I', 'D', 'O', 'P', 'B', 'F', 'G', 'C', 'E', 'K', 'A'}, {'Q', 'L'}, {'N', 'M'}};
	State goal10 = {{'P', 'N', 'H', 'C'}, {'O', 'K', 'Q', 'J', 'D', 'B', 'I'}, {'M', 'E', 'A'}, {'G', 'L'}, {'F'}};
	State sol10 = {{'A', '-'}, {'K', '-'}, {'M', '-'}, {'E', 'M'}, {'A', 'E'}, {'C', '-'}, {'G', '-'}, {'L', 'G'}, {'F', '-'}, {'B', '-'}, {'P', '-'}, {'N', 'P'}, {'O', '-'}, {'K', 'O'}, {'Q', 'K'}, {'D', '-'}, {'I', '-'}, {'H', 'N'}, {'C', 'H'}, {'J', 'Q'}, {'D', 'J'}, {'B', 'D'}, {'I', 'B'}};
	// compare(input10, goal10, sol10);

	State input11 = {{'A', 'B', 'C'}, {'D', 'E'}};
	State goal11 = {{'D', 'E', 'A', 'B', 'C'}};
	State sol11 = {{'C', '-'}, {'B', '-'}, {'A', 'E'}, {'B', 'A'}, {'C', 'B'}};
	// compare(input11, goal11, sol11);

	State input12 = {{'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'}};
	State goal12 = {{'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'}};
	State sol12 = {{'I', '-'}, {'H', 'I'}, {'G', 'H'}, {'F', 'G'}, {'E', 'F'}, {'D', 'E'}, {'C', 'D'}, {'B', 'C'}, {'A', 'B'}};
	// compare(input12, goal12, sol12);

	State input13 = {{'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'}};
	State goal13 = {{'F', 'D', 'C', 'I', 'G', 'A'}, {'B', 'E', 'H'}};
	State sol13 = {{'F', '-'}, {'E', '-'}, {'D', 'F'}, {'C', 'D'}, {'I', 'C'}, {'B', '-'}, {'E', 'B'}, {'H', '-'}, {'H', 'E'}, {'G', 'I'}, {'A', 'G'}};

	compare(input2, goal2, sol2, 2);

	// State input = { {'D', 'I'},{'J', 'K', 'E', 'A', 'H', 'C', 'G'}, {'F', 'B'} };
	// State goal = { {'A', 'C', 'B'},{'G'},{'K'},{'F', 'H', 'I', 'E', 'J', 'D'} };

	// State input({ {'D', 'H', 'B', 'E'},{'G', 'A'},{'I', 'C', 'J', 'F'} });
	// State goal({ {'D', 'C'},{ 'G', 'E' }, {'F', 'B', 'A', 'H' }, {'J', 'I' } });

	// State input = { {'A', 'B', 'C'},{'D', 'E','F'},{'G', 'H', 'I'}};
	// State goal = { {'A', 'B', 'C' ,'D', 'E','F' , 'G', 'H', 'I'} };

	/* vector<pair<string, string>> path;
	 path = BFS_Algorithm(input, goal);

	 cout << "\n\nThe path is: ";
	 for (const auto& pathPair : path)
		cout << "(" << pathPair.first << ", " << pathPair.second << ")\n";*/

	return 0;
}
