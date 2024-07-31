#pragma once

#include <chrono>
#include <set>
#include <queue>
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
	pair<pair<int, double>, vector<string>> bellmanFordSearch(string from, string to);
	void algTesting(); // function purely for testing in the terminal (and not in the gui)
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

	unordered_map<int, int> visited; // key is the inserted article and value is the article that it came from (useful for finding shortest path)
	// predecessor idea inspired by dijkstra's algorithm (slide 3 M11_01_COP3530_F23_Kapoor.pptx)
	queue<int> q;
	vector<pair<int, int>> neighbors; // second is a junk value (weight). this strategy prevents the need to loop through an articles connections.

	visited[fromId] = fromId;
	q.push(fromId);

	while(!q.empty()) // main loop
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
	results.first.second = chrono::duration<double>((endTime - startTime)).count(); // saves time in seconds

	return results;
}

pair<pair<int, double>, vector<string>> Algorithms::dijkstraSearch(string from, string to) // referenced M11_02_COP3530_F23_Kapoor.pptx slide 3 (dijkstra pseudocode)
{
    pair<pair<int, double>, vector<string>> results;
    results.first.first = 0; // Initialize the number of nodes inspected

    int fromId = getID[from]; // Get the ID of the starting node
    int toId = getID[to]; // Get the ID of the destination node

    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();// referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/


    unordered_map<int, int> previous; // To store the previous node for the shortest path
    unordered_map<int, double> distances; // To store the shortest distance to each node
    set<pair<double, int>> nodes; // Set of (distance, node) pairs for Dijkstra's algorithm

    // Initialize distances to infinity for all nodes
    for (const auto &entry : getID)
    {
        distances[entry.second] = numeric_limits<double>::infinity();
    }
    distances[fromId] = 0.0; // Distance to the start node is zero
    nodes.insert({0.0, fromId}); // Insert the start node with distance zero

    while (!nodes.empty()) // main loop
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
            double weight = 1.0; // Weight of the edge
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

    int fromId = getID[from];
    int toId = getID[to];

    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

	// Maps for storing distances, predecessors, and queue status
    unordered_map<int, double> distances; // Distance from the start node
    unordered_map<int, int> predecessors; // Predecessor of each node in the path
    unordered_map<int, bool> inQueue; // Whether a node is currently in the queue

	// Initialize all nodes' distances to infinity, predecessors to -1, and inQueue to false
    for (const auto& id_pair : getID) {
        distances[id_pair.second] = numeric_limits<double>::infinity();
        predecessors[id_pair.second] = -1;
        inQueue[id_pair.second] = false;
    }

    distances[fromId] = 0; // Set the distance to the starting node as 0

    deque<int> q; // Shlok advised the use of a deque. referenced https://cplusplus.com/reference/deque/deque/
    q.push_back(fromId); // Start with the source node

    inQueue[fromId] = true;
    bool distanceUpdated = false;

    for (int i = 0; i < getID.size() - 1; ++i) { // main loop
		distanceUpdated = false;

		for (size_t qSize = q.size(); qSize >0; --qSize) {
			int u = q.front();
			q.pop_front();
			inQueue[u] = false;

			results.first.first++;

			if (u >= wikiDatabase.size()) continue;

			for (const auto& edge : wikiDatabase[u]) {
				int v = edge.first;
				double weight = 1;

				// Check if a shorter path to node v has been found
				if (distances[u] + weight < distances[v]) {
					distances[v] = distances[u] + weight;
					predecessors[v] = u;

					// Only add to the queue if not already in it
					if (!inQueue[v]) {
						q.push_back(v);
						inQueue[v] = true;
					}
					distanceUpdated = true;
				}
			}
		}

		if (!distanceUpdated) {
      		break;
		}
    }

    // Reconstruct path
    if (distances[toId] != numeric_limits<double>::infinity()) {
        for (int at = toId; at != fromId; at = predecessors[at]) {
            results.second.push_back(getTitle[at]);
        }
        results.second.push_back(getTitle[fromId]);
        reverse(results.second.begin(), results.second.end());//// referenced https://cplusplus.com/reference/algorithm/reverse/
    }

    // End timing
    auto endTime = chrono::high_resolution_clock::now();// // referenced https://cplusplus.com/reference/chrono/high_resolution_clock/now/
    results.first.second = chrono::duration<double>((endTime - startTime)).count(); // saves time in seconds

	return results; // Return the results
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

