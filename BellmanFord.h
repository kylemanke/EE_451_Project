#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "Parser.h"

std::string* SerialBellmanFord(Node** AdjList, int numNodes, int startNode, int endNode);
std::string* ParallelBellmanFord(Node** AdjList, int numNodes, int startNode, int endNode);

#endif