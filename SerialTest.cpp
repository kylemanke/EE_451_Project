#include "Parser.h"
#include "ResultWriter.h"

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <random>
#include <queue>
#include <vector>
#include <climits>
#include <time.h>

using namespace std;
using namespace std::chrono;

typedef pair<int, int> pi;

string* djikstra(Node**, int, int, int);
string* djikstra(short unsigned int**, int, int, int);
string* bellmanFord(Node**, int, int, int);
string* bellmanFord(short unsigned int**, int, int, int);

int main(int argc, char* argv[]) {
    // Check usage
    if(argc != 4) {
        cerr << "USAGE: ./SerialTest INPUT_FILE OUTPUT_FILE NUM_ITERATIONS\n";
    }

    // Initialize Data
    Parser parser(argv[1]);

    auto start = high_resolution_clock::now();
    short unsigned int** adjMatrix = parser.parse2AM();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    cout << "Adjacency Matrix parsed in " << duration.count() << " ms\n";

    start = high_resolution_clock::now();
    Node** adjList = parser.parse2LL();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end-start);
    cout << "Adjacency List parsed in " << duration.count() << " ms\n";

    int numNodes = parser.getNumNodes();
    cout << "Number of Nodes: " << numNodes << endl;

    // Initialize Result Writer
    string headerList[8] {"start_node","end_node","d_dist","bf_dist","d_time_matrix","d_time_list","bf_time_matrix","bf_time_list"};
    ResultWriter resultWriter(string(argv[2]), headerList, 8, ",");

    // Go through each iteration
    srand(time(NULL));
    int numIterations;
    stringstream ss(argv[3]);
    ss >> numIterations;
    for(int i=0; i < numIterations; i++) {
        string tempRow[8];
        string rowStr;
        string* retStr;

        // Get the start and end nodes
        int startNode = rand() % numNodes;
        int endNode = rand() % numNodes;
        tempRow[0] = to_string(startNode);
        tempRow[1] = to_string(endNode);

        // Run Djikstras
        retStr = djikstra(adjList, numNodes, startNode, endNode);
        tempRow[2] = retStr[0];
        tempRow[5] = retStr[1];
        delete[] retStr;
        retStr = djikstra(adjMatrix, numNodes, startNode, endNode);
        tempRow[4] = retStr[1];
        delete[] retStr;

        // Run Bellman-Ford
        retStr = bellmanFord(adjList, numNodes, startNode, endNode);
        tempRow[3] = retStr[0];
        tempRow[7] = retStr[1];
        delete[] retStr;
        retStr = bellmanFord(adjMatrix, numNodes, startNode, endNode);
        tempRow[6] = retStr[1];
        delete[] retStr;

        // Print and save the results
        rowStr = resultWriter.write(tempRow);
        cout << i << ": " << rowStr;
    }

    // Deal with the memory management
    for(int i = 0; i < numNodes; i++) {
        delete[] adjMatrix[i];
        Node* currNode=nullptr;
        Node* nextNode=nullptr;
        if(adjList[i] == nullptr)
            continue;
        else {
            currNode = adjList[i];
            nextNode = currNode->next;
            while(nextNode != nullptr) {
                delete currNode;
                currNode = nextNode;
                nextNode = currNode->next;
            }
            delete currNode;
        }
    }
    delete[] adjMatrix;
    delete[] adjList;

    return 1;
}

string* djikstra(Node** adjList, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();

    // Implement Djikstras
    priority_queue<pi, vector<pi>, greater<pi>> pq;
    char* visited = new char[numNodes];
    int* distance = new int[numNodes];
    for(int i=0; i < numNodes; i++) {
        visited[i] = 0;
        distance[i] = INT_MAX;
    }
    distance[startNode] = 0;
    pq.push(pi(0, startNode));
    while(!pq.empty()) {
        pi current = pq.top(); pq.pop();
        if(visited[current.second] == 1)
            continue;
        if(current.second == endNode)
            break;
        visited[current.second] = 1;
        Node* currNode = adjList[current.second];
        while(currNode != nullptr) {
            if (distance[current.second] + currNode->weight < distance[currNode->nodeId] && visited[currNode->nodeId] == 0) {
                distance[currNode->nodeId] = distance[current.second] + currNode->weight;
                pq.push(pi(distance[currNode->nodeId], currNode->nodeId));
            }
            currNode = currNode->next;
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] visited;
    delete[] distance;

    return retStr;
}

string* djikstra(short unsigned int** adjMatrix, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();
    
    // Implement Djikstras
    priority_queue<pi, vector<pi>, greater<pi>> pq;
    char* visited = new char[numNodes];
    int* distance = new int[numNodes];
    for(int i=0; i < numNodes; i++) {
        visited[i] = 0;
        distance[i] = INT_MAX;
    }
    distance[startNode] = 0;
    pq.push(pi(0, startNode));
    while(!pq.empty()) {
        pi current = pq.top(); pq.pop();
        if(visited[current.second] == 1)
            continue;
        if(current.second == endNode)
            break;
        visited[current.second] = 1;
        for(int i = 0; i < numNodes; i++) {
            if((adjMatrix[current.second][i] != 0) && distance[current.second] + adjMatrix[current.second][i] < distance[i] && visited[i] == 0) {
                distance[i] = distance[current.second] + adjMatrix[current.second][i];
                pq.push(pi(distance[i], i));
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] visited;
    delete[] distance;
    
    return retStr;
}

string* bellmanFord(Node** adjList, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();

    // Implement Bellman-Ford
    int* distance = new int[numNodes];
    for(int i = 0; i < numNodes; i++)
        distance[i] = INT_MAX;
    distance[startNode] = 0;
    while(1) {
        bool changed = false;
        for(int i = 0; i < numNodes; i++) {
            Node* currNode = adjList[i];
            while(currNode != nullptr) {
                if(distance[i] == INT_MAX)
                    break;
                else if (distance[i] + currNode->weight < distance[currNode->nodeId]) {
                    changed = true;
                    distance[currNode->nodeId] = distance[i] + currNode->weight;
                }
                currNode = currNode->next;
            }
        }
        if(!changed)
            break;
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] distance;

    return retStr;
}

string* bellmanFord(short unsigned int** adjMatrix, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();
    
    // Implement Bellman-Ford
    int* distance = new int[numNodes];
    for(int i = 0; i < numNodes; i++)
        distance[i] = INT_MAX;
    distance[startNode] = 0;
    while(1) {
        bool changed = false;
        for(int i = 0; i < numNodes; i++) {
            for(int j = 0; j < numNodes; j++) {
                if(adjMatrix[i][j] != 0) {
                    if(distance[i] != INT_MAX && distance[i] + adjMatrix[i][j] < distance[j]) {
                        changed = true;
                        distance[j] = distance[i] + adjMatrix[i][j];
                    }
                }
            }
        }
        if(!changed)
            break;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] distance;

    return retStr;
}
