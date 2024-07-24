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
	pair<pair<int, double>, vector<string>> dijkstraSearch(string from, string to); 
	pair<pair<int, double>, vector<string>> bellmanFordSearch(string from, string to); // need debugging
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
    pair<pair<int, double>, vector<string>> results;
    results.first.first = 0; // Initialize the number of nodes inspected

    int fromId = getID[from]; // Get the ID of the starting node
    int toId = getID[to]; // Get the ID of the destination node

    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();// referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/


    map<int, int> previous; // To store the previous node for the shortest path
    map<int, double> distances; // To store the shortest distance to each node
    set<pair<double, int>> nodes; // Set of (distance, node) pairs for Dijkstra's algorithm

    // Initialize distances to infinity for all nodes
    for (const auto &entry : getID)
    {
        distances[entry.second] = numeric_limits<double>::infinity();
    }
    distances[fromId] = 0.0; // Distance to the start node is zero
    nodes.insert({0.0, fromId}); // Insert the start node with distance zero

    while (!nodes.empty())
    {
        auto smallest = *nodes.begin(); // Get the node with the smallest distance
        nodes.erase(nodes.begin()); // Remove the node from the set
        int currentPage = smallest.second; // Get the node ID

        results.first.first++; // Increment the number of nodes inspected

        if (currentPage == toId) // if destination node stop
        {
            break;
        }

        // Iterate through all neighbors of the current node
        for (const auto &neighbor : wikiDatabase[currentPage])
        {
            int neighborId = neighbor.first; // Neighbor node ID
            double weight = neighbor.second; // Weight of the edge
            double alt = distances[currentPage] + weight; // Calculate alternative path distance
            if (alt < distances[neighborId]) // If a shorter path is found
            {
                nodes.erase({distances[neighborId], neighborId}); // Remove the old distance
                distances[neighborId] = alt; // Update to the new shorter distance
                previous[neighborId] = currentPage; // Update the previous node
                nodes.insert({alt, neighborId}); // Insert the updated distance
            }
        }
    }

    // If a path was found, reconstruct the path
    if (previous.find(toId) != previous.end() || fromId == toId)
    {
        int temp = toId;
        while (temp != fromId)
        {
            results.second.push_back(getTitle[temp]); // Add the node to the path
            temp = previous[temp]; // Move to the previous node
        }
        results.second.push_back(getTitle[temp]); // Add the start node
        reverse(results.second.begin(), results.second.end()); // referenced https://cplusplus.com/reference/algorithm/reverse/
    }

    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/
    results.first.second = chrono::duration<double>((endTime - startTime)).count(); // saves time in seconds

    return results; // Return the results
}

pair<pair<int, double>, vector<string>> Algorithms::bellmanFordSearch(string from, string to) {
    pair<pair<int, double>, vector<string>> results;
    results.first.first = 0; // Number of nodes inspected

    if (getID.find(from) == getID.end() || getID.find(to) == getID.end()) {
        cout << "One of the specified nodes does not exist." << endl;
        return results;
    }

    int fromId = getID[from];
    int toId = getID[to];

    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/

    // Distance and predecessor maps
    unordered_map<int, double> distances;
    unordered_map<int, int> predecessors;

    // Initialize distances and predecessors
    for (auto& id_pair : getID) {
        distances[id_pair.second] = numeric_limits<double>::infinity();
        predecessors[id_pair.second] = -1;
    }

    distances[fromId] = 0;

    // Relax edges up to V-1 times
    bool updated;
    for (int i = 0; i < getID.size() - 1; i++) {
        updated = false;
        for (int u = 1; u < wikiDatabase.size(); u++) {
            for (auto& edge : wikiDatabase[u]) {
                int v = edge.first;
                double weight = edge.second;
                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    updated = true;
                }
            }
        }
        if (!updated) break;
    }

    // Check for negative weight cycles
    for (int u = 1; u < wikiDatabase.size(); u++) {
        for (auto& edge : wikiDatabase[u]) {
            int v = edge.first;
            double weight = edge.second;
            if (distances[u] + weight < distances[v]) {
                cout << "Graph contains a negative weight cycle." << endl;
                return results;
            }
        }
    }

    // Reconstruct path if possible
    if (distances[toId] != numeric_limits<double>::infinity()) {
        for (int at = toId; at != -1; at = predecessors[at]) {
            results.second.push_back(getTitle[at]);
            results.first.first++; // Increment the node count here if only counting the path
        }
        reverse(results.second.begin(), results.second.end());
    }

    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now(); // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/
    results.first.second = chrono::duration<double>((endTime - startTime)).count(); // saves time in seconds
    return results;
}


void Algorithms::algTesting()
{
    string from = "World War II"; // enter starting from page here
    string to = "Tony Hawk's Pro Skater 2"; // enter target page here
    pair<pair<int, double>, vector<string>> results;

    // Breadth-First Search
    results = breadthFirstSearch(from, to);

    cout << "\nBFS TESTING: \n\nStarting at: \"" << from <<"\"\nSearching for: \"" << to << "\"\n";
    cout << "Shortest Path Length: " << to_string(results.second.size()) << endl;
    cout << "Number of Pages Inspected: " << to_string(results.first.first) << endl;
    cout << "Runtime: " << to_string(results.first.second) << " seconds" <<endl;

    if(results.second.size() == 0)
    {
        cout << "Page Connection Not Found!";
    }
    else
    {
        for(int i = 0; i < results.second.size() - 1; i++) // prints all but final element
        {
            cout << results.second[i] << " -> ";
        }
        cout << results.second[results.second.size() - 1]; // prints final element with no arrow
    }

    // Dijkstra's Algorithm
    results = dijkstraSearch(from, to);

    cout << "\n\nDijkstra's TESTING: \n\nStarting at: \"" << from <<"\"\nSearching for: \"" << to << "\"\n";
    cout << "Shortest Path Length: " << to_string(results.second.size()) << endl;
    cout << "Number of Pages Inspected: " << to_string(results.first.first) << endl;
    cout << "Runtime: " << to_string(results.first.second) << " seconds" <<endl;

    if(results.second.size() == 0)
    {
        cout << "Page Connection Not Found!";
    }
    else
    {
        for(int i = 0; i < results.second.size() - 1; i++) // prints all but final element
        {
            cout << results.second[i] << " -> ";
        }
        cout << results.second[results.second.size() - 1]; // prints final element with no arrow
    }

    // Bellman-Ford Algorithm
    results = bellmanFordSearch(from, to);

    cout << "\n\nBellman-Ford TESTING: \n\nStarting at: \"" << from <<"\"\nSearching for: \"" << to << "\"\n";
    cout << "Shortest Path Length: " << to_string(results.second.size()) << endl;
    cout << "Number of Pages Inspected: " << to_string(results.first.first) << endl;
    cout << "Runtime: " << to_string(results.first.second) << " seconds" <<endl;

    if(results.second.size() == 0)
    {
        cout << "Page Connection Not Found!";
    }
    else
    {
        for(int i = 0; i < results.second.size() - 1; i++) // prints all but final element
        {
            cout << results.second[i] << " -> ";
        }
        cout << results.second[results.second.size() - 1]; // prints final element with no arrow
    }
}

