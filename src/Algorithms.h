#pragma once

#include <chrono>
#include <set>
#include <queue>
#include <map>
#include <ctime>
#include <algorithm>

#include "WikiDatabase.cpp"

class Algorithms : public WikiDatabase
{
private:
	// add any private helper functions here:

public:
	// NOTE FOR ALL 3 ALGORITHMS:
	// returns a pair containing another pair in first, and a vector of strings in second that contains the path between the "from" and "to" articles.
	// the pair in first contains the number of nodes inspected as an int (first) and the runtime of the function as a double (second).

	pair<pair<int, double>, vector<string>> breadthFirstSearch(string from, string to);
	pair<pair<int, double>, vector<string>> dijkstraSearch(string from, string to); // todo: write me
	pair<pair<int, double>, vector<string>> bellmanFordSearch(string from, string to); // todo: write me
	void algTesting(); // function purely for testing purposes
};

// Class Function Definitions ---------------------

pair<pair<int, double>, vector<string>> Algorithms::breadthFirstSearch(string from, string to) // uses modified version of algorithm found in M10_08_COP3530_F23_Kapoor_accessible.pdf (Module 10 Graph Traversal slides) (page 4)
{
	pair<pair<int, double>, vector<string>> results;
	results.first.first = 0;

	int fromId = getID[from];
	int toId = getID[to];
	bool found = false;

	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/

	map<int, int> visited; // key is the inserted article and value is the article that it came from (useful for finding shortest path)
	queue<int> q;
	visited[fromId] = fromId;
	vector<pair<int, int>> neighbors; // second is a junk value (weight). this strategy prevents the need to loop through an articles connections.

	q.push(fromId);

	while(!q.empty())
	{
		int currentPage = q.front();
		results.first.first ++; // increment nodes inspected
		q.pop();

		neighbors = wikiDatabase[currentPage];
		sort(neighbors.begin(), neighbors.begin() + neighbors.size());
		for(int i = 0; i < neighbors.size(); i++)
		{
			if(visited.find(neighbors[i].first) == visited.end())
			{
				visited[neighbors[i].first] = currentPage;
				if(neighbors[i].first == toId)
				{
					found = true;
					break;
				}
				q.push(neighbors[i].first);
			}
		}

		if(found) break;
	}

	if(found)
	{
		int temp = toId;
		while(temp != fromId)
		{
			results.second.push_back(getTitle[temp]);
			temp = visited[temp]; // sets temp to its predecessor
		}
		results.second.push_back(getTitle[temp]); // adds fromId

		reverse(results.second.begin(), results.second.end()); // referenced https://cplusplus.com/reference/algorithm/reverse/
	}

	chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/
	results.first.second = chrono::duration<double>((endTime - startTime)).count(); // saves time in seconds (should this be different? milliseconds perhaps?)

	return results;
}

pair<pair<int, double>, vector<string>> Algorithms::dijkstraSearch(string from, string to)
{
	// todo: implement dijkstra's alg here. Follow BFS implementation for the return format.
	return pair<pair<int, double>, vector<string>>();
}

pair<pair<int, double>, vector<string>> Algorithms::bellmanFordSearch(string from, string to)
{
	// todo: implement bellman ford alg here. Follow BFS implementation for the return format.
	return pair<pair<int, double>, vector<string>>();
}

void Algorithms::algTesting()
{
	string from = "World War II"; // enter stating from page here
	string to = "Tony Hawk's Pro Skater 2"; // enter target page here
	pair<pair<int, double>, vector<string>> results = breadthFirstSearch(from, to);

	cout << "\nBFS TESTING: \n\nStarting at: \"" << from <<"\"\nSearching for: \"" << to << "\"\n";
	cout << "Shortest Path Length: " << to_string(results.second.size()) << endl;
	cout << "Number of Pages Inspected: " << to_string(results.first.first - 1) << endl;
	cout << "Runtime: " << to_string(results.first.second) << " seconds" <<endl;

	if(results.second.size() == 0)
	{
		cout << "Page Connection Not Found!";
		return;
	}
	for(int i = 0; i < results.second.size() - 1; i++) // prints all but final element
	{
		cout << results.second[i] << " -> ";
	}
	cout << results.second[results.second.size() - 1]; // prints final element with no arrow

	// todo: add more testing code for dijkstra's and bellman ford as they get finished here.
	// follow the printing layout used in the BFS testing for consistency.
}
