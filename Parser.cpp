#include "Parser.h"
#include <sstream>

using namespace std;

Parser::Parser(string fName) {
    file.open(fName);
}

void Parser::addEdge(Node** list, int start, int dest) {
    Node* newNode = new Node();
    newNode->nodeId = dest;

    if(list[start] == nullptr)
        list[start] = newNode;
    else {
        Node* currNode = list[start];
        while (currNode->next != nullptr)
            currNode = currNode->next;
        currNode->next = newNode;
    }
}

int Parser::getNumNodes() {
    return numNodes;
}

Node** Parser::parse2LL() {
    // Read in the first line
    string currLine;
    getline(file, currLine);
    stringstream firstLine(currLine);
    firstLine >> numNodes;

    // Create the array
    Node** adjacencyList = new Node*[numNodes];
    for(int i = 0; i < numNodes; i++)
        adjacencyList[i] = nullptr;
    int start, dest;
    while(getline(file, currLine)) {
        stringstream line(currLine);
        line >> start >> dest;
        addEdge(adjacencyList, start, dest);
    }

    return adjacencyList;
}

char** Parser::parse2AM() {
    // Read in the first line
    string currLine;
    getline(file, currLine);
    stringstream firstLine(currLine);
    firstLine >> numNodes;

    // Create the matrix
    int compressedSize = (numNodes/8);
    char** adjMatrix = new char*[numNodes];
    for(int i = 0; i < numNodes; i++) {
        adjMatrix[i] = new char[compressedSize];
        for(int j = 0; j < compressedSize; j++) {
            adjMatrix[i][j] = (char)0;
        }
    }
    int start, dest;
    while(getline(file, currLine)) {
        stringstream line(currLine);
        line >> start >> dest;
        adjMatrix[start][dest/8] = adjMatrix[start][dest/8] | (0x80 >> (dest%8));
    }

    return adjMatrix;
}