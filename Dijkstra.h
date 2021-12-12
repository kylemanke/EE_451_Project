#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Parser.h"

std::string* SerialDijkstra(Node** AdjList, int numNodes, int startNode, int endNode);
std::string* ParallelDijkstra(Node** AdjList, int numNodes, int startNode, int endNode, int p);

#endif
