#include <chrono>
#include <climits>
#include <omp.h>
#include <iostream>
#include "BellmanFord.h"

using namespace std;
using namespace std::chrono;

string* ParallelBellmanFord(Node** AdjList, int numNodes, int startNode, int endNode, int p) {
    string* retStr = new string[2];
    auto start = high_resolution_clock::now();

    // Step 1: Set number of threads
    omp_set_num_threads(p);

    // Step 2: Determine workload for each thread
    int* start_pos = new int[p];
    int* end_pos = new int[p];
    int bucketSize = numNodes/p;
    
    #pragma omp parallel for
    for(int i=0; i < p; i++) {
        start_pos[i] = bucketSize * i;
        end_pos[i] = bucketSize * (i+1);
        if (i == p-1)
            end_pos[i] = numNodes;
    }

    // Step 3: Initialize the distances
    int* distance = new int[numNodes];
    #pragma omp parallel for
    for (int i=0; i < numNodes; i++)
        distance[i] = INT_MAX;
    distance[startNode] = 0;

    // Step 4: Run the algo
    bool globalChange;
    bool* localChange = new bool[p];
    #pragma omp parallel
    {
        int threadNum = omp_get_thread_num();
        for (int i=0; i < numNodes-1; i++) {
            localChange[threadNum] = false;
            for (int currIndex = start_pos[threadNum]; currIndex < end_pos[threadNum]; currIndex++) {
                // Go through all neighboring nodes
                Node* currNode = AdjList[currIndex];
                while(currNode != nullptr) {
                    int old_distance = distance[currNode->nodeId];
                    if (old_distance != INT_MAX) {
                        int new_distance = old_distance + currNode->weight;
                        if (new_distance < distance[currIndex]) {
                            localChange[threadNum] = true;
                            distance[currIndex] = new_distance;
                        }
                    }
                    currNode = currNode->next;
                }
            }

            #pragma omp barrier
            #pragma omp single
            {
                globalChange = false;
                for (int boolI = 0; boolI < p; boolI++) {
                    globalChange |= localChange[boolI];
                }
            }
            if(!globalChange)
                break;
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    retStr[0] = to_string(distance[endNode]);
    retStr[1] = to_string(duration.count());

    // Clear up memory
    delete[] start_pos;
    delete[] end_pos;
    delete[] localChange;

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
