#include <chrono>
#include <climits>
#include "BellmanFord.h"

using namespace std;
using namespace std::chrono;

string* ParallelBellmanFord(Node** AdjList, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();

    // Implement BellmanFord with OpenMP
    int* distance = new int[numNodes];
    #pragma omp parallel for
    for (int i = 0; i < numNodes; i++)
        distance[i] = INT_MAX;
    
    distance[startNode] = 0;
    bool overAllChanged = true;
    while(overAllChanged) {
        overAllChanged = false;
        #pragma omp parallel for
        for(int i = 0; i < numNodes; i++) {
            bool localChange = false;
            Node* currNode = AdjList[i];
            while(currNode != nullptr) {
                if (distance[i] == INT_MAX)
                    break;
                else if (distance[i] + currNode->weight < distance[currNode->nodeId]) {
                    localChange = true;
                    distance[currNode->nodeId] = distance[i] + currNode->weight;
                }
                currNode = currNode->next;
            }
            overAllChanged |= localChange;
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] distance;

    return retStr;
}

string* SerialBellmanFord(Node** AdjList, int numNodes, int startNode, int endNode) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();

    // Implement a Serial Version of BellmanFord
    int* distance = new int[numNodes];
    for (int i=0; i < numNodes; i++)
        distance[i] = INT_MAX;
    
    distance[startNode] = 0;
    bool changed = true;
    while(changed) {
        changed = false;
        for(int i=0; i < numNodes; i++) {
            Node* currNode = AdjList[i];
            while(currNode != nullptr) {
                if (distance[i] == INT_MAX)
                    break;
                else if (distance[i] + currNode->weight < distance[currNode->nodeId]) {
                    changed = true;
                    distance[currNode->nodeId] = distance[i] + currNode->weight;
                }
                currNode = currNode->next;
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    delete[] distance;

    return retStr;
}
