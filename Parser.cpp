#include "Parser.h"
#include <sstream>
#include <random>

using namespace std;

#define SEED 100

Parser::Parser(string fName) {
    fileName = fName;
}

void Parser::addEdge(Node** list, int start, int dest, short unsigned int weight) {
    Node* newNode = new Node();
    newNode->nodeId = dest;
    newNode->weight = weight;

    if(list[start] == nullptr)
        list[start] = newNode;
    else {
        Node* currNode = list[start];
        if(currNode->nodeId == dest) {
            currNode->weight = weight;
            return;
        }
        while (currNode->next != nullptr) {
            currNode = currNode->next;
            if(currNode->nodeId == dest) {
                currNode->weight = weight;
                return;
            }
        }
        currNode->next = newNode;
    }
}

int Parser::getNumNodes() {
    return numNodes;
}

Node** Parser::parse2LL() {
    // Read in the first line
    ifstream file(fileName);
    string currLine;
    getline(file, currLine);
    stringstream firstLine(currLine);
    firstLine >> numNodes;

    // Create the array
    srand(SEED);
    Node** adjacencyList = new Node*[numNodes];
    for(int i = 0; i < numNodes; i++)
        adjacencyList[i] = nullptr;
    int start, dest;
    while(getline(file, currLine)) {
        stringstream line(currLine);
        line >> start >> dest;
        short unsigned int weight = rand() % 256;
        addEdge(adjacencyList, start, dest, weight);
        addEdge(adjacencyList, dest, start, weight);
    }

    file.close();

    return adjacencyList;
}

short unsigned int** Parser::parse2AM() {
    // Read in the first line
    ifstream file(fileName);
    string currLine;
    getline(file, currLine);
    stringstream firstLine(currLine);
    firstLine >> numNodes;

    // Create the matrix
    srand(SEED);
    short unsigned int** adjMatrix = new short unsigned int*[numNodes];
    for(int i = 0; i < numNodes; i++) {
        adjMatrix[i] = new short unsigned int[numNodes];
        for(int j = 0; j < numNodes; j++) {
            adjMatrix[i][j] = 0;
        }
    }
    int start, dest;
    while(getline(file, currLine)) {
        stringstream line(currLine);
        line >> start >> dest;
        short unsigned int weight = rand() % 256;
        adjMatrix[start][dest] = weight;
        adjMatrix[dest][start] = weight;
    }

    file.close();

    return adjMatrix;
}