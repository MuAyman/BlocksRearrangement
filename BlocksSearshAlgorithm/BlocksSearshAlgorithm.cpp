#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <sstream>

using namespace std;

struct Block
{
	char letter;
	bool isgoal;

	Block(char let) : letter(let), isgoal(0){};
};

using State = vector<vector<Block>>;	    // 2D vector to store the stacks of blocks
using Moves = vector<pair<string, string>>; // pair of block and its destination

class StateSpace
{	   // we can of course make getters and setters but
public: // not worth it in the context of a test task
	State state;
	Moves move;
	int fixedCount; // how close you are to the goal

	StateSpace() : state({}), move({}), fixedCount(0) {}
	StateSpace(State s) : state(s), move({}), fixedCount(0) {}
	StateSpace(State s, Moves m) : state(s), move(m), fixedCount(0) {}
	StateSpace(State s, Moves m, int f) : state(s), move(m), fixedCount(f) {}
};

// function to compress state vectors to strings (with sorted stacks meaning
// if state = DB/AC is will be AC/DB (same block congiuration but sorted to prevent repetition
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
			if (s[j + 1].empty()) // moving empty states to the end of the state vector
				continue;
			else if (s[j].empty()) // moving empty states to the end of the state vector
			{
				swap(s[j], s[j + 1]); // swaping the two stacks of the state vector
				swapped = 1;
			}
			else if (s[j][0].letter > s[j + 1][0].letter) // comparing the tables (elements on the table) and sorting accourrding to them
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
			wholeState += blocks.letter;
		wholeState += "/"; // separate stacks from each other
	}
	return wholeState;
}

// Function to check if the current state matches the goal state
bool IsGoal(const State &current, const State &goal)
{
	int flag = 0, totalBloacksNum = 0; // to count the num of similar stacks
	for (const auto &curStack : current)
	{
		totalBloacksNum += curStack.size(); // counting the total num of blocks
		for (const auto &block : curStack)
		{
			if (block.isgoal) // conting the num of block in goal pos
				++flag;
		}
	}

	if (flag == totalBloacksNum)
		return true; // goal reached if all block are in goal pos
	else
		return false;
}

// function to generate the next possible states from the current state
vector<StateSpace> getNextStates(const StateSpace &current)
{
	vector<StateSpace> nextStates;

	for (int i = 0; i < current.state.size(); ++i)
	{
		if (!current.state[i].empty())
		{ // Current stack not empty
			Block topBlock = current.state[i].back();

			// moving the top block of one stack to another stack
			for (int j = 0; j < current.state.size(); ++j)
			{
				if (i != j)
				{ // check that its a different stack
					State newState = current.state;
					int newFixed = current.fixedCount;
					Moves newPath = current.move;
					newPath.push_back(make_pair(string(1, topBlock.letter), current.state[j].empty() ? "Table" : string(1, current.state[j].back().letter)));
					newState[j].push_back(topBlock);
					newState[i].pop_back();
					nextStates.push_back({newState, newPath, newFixed});
				}
			}
			// moving the top block to an existing empty stack before adding new stack
			bool emptyStackUsed = false;
			for (int j = 0; j < current.state.size(); ++j)
			{
				if (current.state[j].empty())
				{ // if you found an empty stack already, use it
					State newState = current.state;
					int newFixed = current.fixedCount;
					Moves newPath = current.move;
					newState[j].push_back({topBlock}); // push back in the empty stack
					newPath.push_back(make_pair(string(1, topBlock.letter), "Table"));
					newState[i].pop_back();
					nextStates.push_back({newState, newPath, newFixed});
					emptyStackUsed = true;
					break;
				}
			}
			// if no empty stack found, create a new stack
			if (!emptyStackUsed)
			{
				State newState = current.state;
				int newFixed = current.fixedCount;
				Moves newPath = current.move;
				newState.push_back({topBlock});
				newPath.push_back(make_pair(string(1, topBlock.letter), "Table"));
				newState[i].pop_back();
				nextStates.push_back({newState, newPath, newFixed});
			}
		}
	}
	return nextStates;
}

// function to fix all the blocks that reach their goal pos and update state's fixedCount attribute
void countMatches(StateSpace &current, const State &goal)
{
	current.fixedCount = 0; // insuring it hasnt been counted before so that we double the count
	// check if any of the tables (blocks on table) are in the right place
	for (auto &curStack : current.state) // without const as we are marking the DONE blocks
		if (!curStack.empty())
			for (const auto &goalStack : goal)
				if (curStack.front().letter == goalStack.front().letter)
				{
					curStack[0].isgoal = true; // marking the matched element that it is fixed and not to be moved
					++current.fixedCount;
					int counter = 0;																	// to iterate over the rest of the stack
					while (curStack[counter].isgoal && counter < curStack.size() - 1 && counter < goalStack.size() - 1) // extend the checking to the elements above
					{																				// making sure that the counter doesnt go beyond the current or goal stacks
						++counter;
						if (curStack[counter].letter == goalStack[counter].letter)
						{
							curStack[counter].isgoal = true; // mark done
							++current.fixedCount;
						}
					}
				}
}

// BFS algorithm to find the optimal path to reach the goal state
Moves BFS_Algorithm(const State &start, const State &goal)
{
	queue<StateSpace> BFSq;		 // queue for BFS
	unordered_set<string> visited; // unordered map to enhance the efficacy of the searching in it every iteration

	StateSpace startState(start);
	BFSq.push(startState);
	visited.insert(toString(start)); // store the visited states as a string for ease of access and comparison

	while (!BFSq.empty())
	{
		StateSpace current = BFSq.front();
		BFSq.pop();
		if (IsGoal(current.state, goal))
			return current.move;

		countMatches(current, goal);			 // count the num of blocks in goal pos to use in the priority queue
		visited.insert(toString(current.state)); // update the visited states

		vector<StateSpace> nextStates = getNextStates(current); // getting the next possible states from the current one to discover

		for (auto &nextState : nextStates) // looping on them to check if visited before and count the matches
		{
			string stateString = toString(nextState.state);
			if (visited.find(stateString) == visited.end())
			{ // check if visited before
				countMatches(nextState, goal);
				BFSq.push(nextState);
			}
		}
	}
	return {}; // return empty path if no path is found
}

void compare(State in, State goal, State sol, int n)
{
	vector<pair<string, string>> path = BFS_Algorithm(in, goal);
	cout << "\n\t" << n << ". my sol moves: " << path.size() << "\n\t";
	for (int i = 0; i < path.size(); ++i)
		cout << "\t" << i + 1 << ". Move " << path[i].first << " to " << path[i].second << "\n\t";

	cout << "\n\toptimal sol moves: " << sol.size() << "\n\t";
	for (int i = 0; i < sol.size(); ++i)
		cout << "\t" << i + 1 << ". Move " << sol[i].front().letter << " to " << sol[i].back().letter << "\n\t";
}

int main()
{
	cout << "all the sol moves are 1-indexed to match the format of the problem statement\n";

	// initialize the start and goal states
	State start = {{Block('A'), Block('B')}, {Block('C'), Block('D')}, {}};
	State goal = {{Block('A')}, {Block('B')}, {Block('C'), Block('D')}};
	State sol = {{Block('A'), Block('B')}, {Block('C')}, {Block('D')}};

	// compare my solution with the optimal solution
	compare(start, goal, sol, 1);

	// other test cases (1 per line)
	compare({{Block('A'), Block('B')}, {Block('C')}, {Block('D')}}, {{Block('A')}, {Block('B')}, {Block('C'), Block('D')}}, {{Block('A'), Block('B')}, {Block('C'), Block('D')}}, 2);

	return 0;
}
