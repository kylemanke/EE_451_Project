#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>

struct Node {
    int nodeId;
    Node* next = nullptr;
};

class Parser {
public:
    Parser(std::string fname);
    Node** parse2LL();
    char** parse2AM();
    int getNumNodes();
private:
    void addEdge(Node** list, int start, int dest);
    std::string fileName;
    int numNodes;
};

#endif