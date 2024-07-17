#include <chrono>
#include <set>
#include <queue>

#include "WikiDatabase.cpp"

class Algorithms : public WikiDatabase
{
private:


public:
	pair<pair<int, double>, vector<string>> breadthFirstSearch(string from, string to);
	// returns a pair containing another pair in first, and a vector of strings in second that contains the path between the "from" and "to" articles.
	// the pair in first contains the number of nodes inspected as an int (first) and the runtime of the function as a double (second).
};

// Class Function Definitions ---------------------

pair<pair<int, double>, vector<string>> Algorithms::breadthFirstSearch(string from, string to) // uses modified version of algorithm found in M10_08_COP3530_F23_Kapoor_accessible.pdf (Module 10 Graph Traversal slides) (page 4)
{
	// todo: Finish BFS function

	pair<pair<int, double>, vector<string>> results;
	int fromId = getID[from];
	int toId = getID[to];

	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/

	std::set<int> visited;
	std::queue<int> q;
	visited.insert(fromId);
	q.push(fromId);

	while(!q.empty())
	{
		int currentPage = q.front();
		results.first.first ++; // increment nodes inspected

		if(currentPage = toId)
		{

		}
		q.pop();

		vector<string> neighbors = graph[currentPage];
		std::sort(neighbors.begin(), neighbors.begin() + neighbors.size());
		for(string v: neighbors)
		{
			if(visited.count(v) == 0)
			{
				visited.insert(v);
				q.push(v);
			}
		}
	}

	return results;
}
