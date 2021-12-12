#include <chrono>
#include <climits>
#include <omp.h>
#include <queue>
#include <iostream>
#include "Dijkstra.h"

using namespace std;
using namespace std::chrono;

typedef pair<int, int> pi;

string* ParallelDijkstra(Node** AdjList, int numNodes, int startNode, int endNode, int p) {
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
    bool endFound = false;
    while(!pq.empty()) {
        vector<int> minList;
        int currMin = pq.top().first;
        while(!pq.empty() && pq.top().first == currMin) {
            pi current = pq.top(); pq.pop();
            if(visited[current.second] == 1)
                continue;
            if(current.second == endNode) {
                endFound = true;
                break;
            }
            visited[current.second] = 1;
            minList.push_back(current.second);
        }
        if (endFound)
            break;
        #pragma omp parallel num_threads(p)
        {
            #pragma omp for
            for(int i=0; i < minList.size(); i++) {
                Node* currNode = AdjList[minList[i]];
                while (currNode != nullptr) {
                    if (visited[currNode->nodeId] == 0) {
                        if (distance[minList[i]] + currNode->weight < distance[currNode->nodeId]) {
                            #pragma omp critical 
                            {
                                distance[currNode->nodeId] = distance[minList[i]] + currNode->weight;
                                pq.push(pi(distance[currNode->nodeId], currNode->nodeId));
                            }
                        }
                    }
                    currNode = currNode->next;
                }
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

string* SerialDijkstra(Node** AdjList, int numNodes, int startNode, int endNode) {
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
        Node* currNode = AdjList[current.second];
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