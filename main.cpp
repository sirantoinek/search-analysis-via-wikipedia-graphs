#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
using namespace std;

void deserializeIDMap(unordered_map<string, int>& map, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "You're bad. Go next. (ID map file not opened successfully)";
        return;
    }

    size_t size;
    size_t length;

    file.read(reinterpret_cast<char*>(&size), sizeof(size)); // method inspired by .tga file reading
    string key;
    int value;

    while (size-- > 0) {
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        key.resize(length);

        file.read(&key[0], length);
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        map[key] = value;
    }
    file.close();
}


void deserializeVec(vector<vector<pair<int, int>>>& wikiDatabase, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "You're bad. Go next. (database file not opened successfully)";
        return;
    }

    size_t outer_size;
    size_t inner_size;
    int first;
    int second;

    file.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size)); // method inspired by .tga file reading
    wikiDatabase.resize(outer_size); // outer_size and inner_size had to be written during serialization to make this possible, everything else is self-explanatory

    for (auto& innerVec : wikiDatabase) {
        file.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));
        innerVec.resize(inner_size);

        for (auto& pair : innerVec) {
            file.read(reinterpret_cast<char*>(&first), sizeof(first));
            file.read(reinterpret_cast<char*>(&second), sizeof(second));
            pair = {first, second};
        }
    }
    file.close();
}


void deserializeStringMap(unordered_map<int, string>& map, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "You're bad. Go next. (string map file not opened successfully)";
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    while (size-- > 0) {
        size_t length;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        string key(length, '\0');
        file.read(&key[0], length);

        int value;
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        map[value] = key;
    }
    file.close();
}


void useful4Visualization(vector<vector<pair<int,int>>> wikiDatabase, unordered_map<string,int> pages){ // purely non-functional function

    for(int i = 0; i < 26; i++){ // these for loops are useful reference for how to access each part of the database
        cout << "Page #" << i << " Connections: " << endl;
        for(auto & j : wikiDatabase[i]){
            cout << j.first << " ";
        }
        cout << endl;
    }

    /*

    string wikiPage = "Simpson Mutambanengwe";
    for(int i = 0; i < wikiDatabase[pages[wikiPage]].size(); i++){ // could be replaced with for(auto & i : wikiDatabase[pages[wikiPage]]){}
        cout << wikiDatabase[pages[wikiPage]][i].first << " ";
    } // shows all the connections that Simpson Mutambanengwe leads to

    */
}


int main() {

    string stringMapFile = "DOWNLOAD LOCATION";
    string IDMapFile = "DOWNLOAD LOCATION";
    string vecFile = "DOWNLOAD LOCATION";

    unordered_map<string, int> getID;
    unordered_map<int, string> getTitle;
    vector<vector<pair<int, int>>> wikiDatabase;

    deserializeIDMap(getID, stringMapFile);
    deserializeStringMap(getTitle, IDMapFile);
    deserializeVec(wikiDatabase, vecFile);

    return 0;
}
