#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
using namespace std;

struct WikiDatabase
{
	string stringMapFile = "bin/getStringMap.bin";
	string IDMapFile = "bin/getIDmap.bin";
	string vecFile = "bin/wikiDatabase.bin";

	unordered_map<string, int> getID; // Useful for converting a title to it's associated ID
	unordered_map<int, string> getTitle; // Useful for converting an ID to it's associated title
	vector<vector<pair<int, int>>> wikiDatabase; // Vector of connection vectors.
	// Connection vectors hold connections with the connecting article id as .first, and the weight of the connection as .second

	WikiDatabase()
	{
		deserializeIDMap();
		deserializeStringMap();
		deserializeVec();
	}

private:

	void deserializeIDMap() // converts getStringMap.bin into usable data
	{
		ifstream file(stringMapFile, ios::binary);
		if (!file.is_open())
		{
			cerr << "ID map file not opened successfully.";
			return;
		}

		size_t size;
		size_t length;

		file.read(reinterpret_cast<char*>(&size), sizeof(size)); // method inspired by .tga file reading
		string key;
		int value;

		while (size-- > 0)
		{
			file.read(reinterpret_cast<char*>(&length), sizeof(length));
			key.resize(length);

			file.read(&key[0], length);
			file.read(reinterpret_cast<char*>(&value), sizeof(value));
			getID[key] = value;
		}
		file.close();
	}

	void deserializeStringMap() // converts getIDmap.bin into usable data
	{
		ifstream file(IDMapFile, ios::binary);
		if (!file.is_open())
		{
			cerr << "String map file not opened successfully.";
			return;
		}

		size_t size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));

		while (size-- > 0)
		{
			size_t length;
			file.read(reinterpret_cast<char*>(&length), sizeof(length));
			string key(length, '\0');
			file.read(&key[0], length);

			int value;
			file.read(reinterpret_cast<char*>(&value), sizeof(value));
			getTitle[value] = key;
		}
		file.close();
	}

	void deserializeVec() // converts wikiDatabase.bin into usable data
	{
		ifstream file(vecFile, ios::binary);
		if (!file.is_open())
		{
			cerr << "Database file not opened successfully.";
			return;
		}

		size_t outer_size;
		size_t inner_size;
		int first;
		int second;

		file.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size)); // method inspired by .tga file reading from COP3503C Project 2 ImageProcessing
		wikiDatabase.resize(outer_size); // outer_size and inner_size had to be written during serialization to make this possible

		for (auto& innerVec : wikiDatabase)
		{
			file.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));
			innerVec.resize(inner_size);

			for (auto& pair : innerVec)
			{
				file.read(reinterpret_cast<char*>(&first), sizeof(first));
				file.read(reinterpret_cast<char*>(&second), sizeof(second));
				pair = {first, second};
			}
		}
		file.close();
	}
};