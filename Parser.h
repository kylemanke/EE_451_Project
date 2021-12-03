#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>

struct Node {
    int nodeId;
    short unsigned int weight;
    Node* next = nullptr;
};

class Parser {
public:
    Parser(std::string fname);
    Node** parse2LL();
    short unsigned int** parse2AM();
    int getNumNodes();
private:
    void addEdge(Node** list, int start, int dest, short unsigned int weight);
    std::string fileName;
    int numNodes;
};

#endif