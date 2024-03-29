/* C++ includes */
#include <chrono>
#include <iostream>
#include <sstream>
#include <random>
#include <time.h>
#include <omp.h>

/* Custom includes */
#include "Parser.h"
#include "Dijkstra.h"

using namespace std;
using namespace std::chrono;

#define NUM_THREADS 4

int main(int argv, char* argc[]) {
    // Check args: ./Dijkstra_Test INPUTFILE NUM_ITERATIONS
    if(argv != 3) {
        cout << "USAGE: ./Dijkstra_Test INPUT_FILE NUM_ITERATIONS\n";
        return -1;
    }

    // Get number of iterations
    int numIterations;
    stringstream ss(argc[2]);
    ss >> numIterations;

    // Open the file
    Parser parser(argc[1]);
    auto start = high_resolution_clock::now();
    Node** AdjList = parser.parse2LL();
    int numNodes = parser.getNumNodes();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    cout << "Parsed to Adjacency List in " << duration.count() << "ms\n";
    cout << "Found: " << numNodes << " nodes\n";
    cout << "Num threads: " << NUM_THREADS << endl;

    // Do the iterations 
    srand(time(NULL));
    for(int i = 0; i < numIterations; i++) {
        // Get the start and end nodes
        int startNode = rand() % numNodes;
        int endNode = rand() % numNodes;

        // Run the iteration
        cout << i << ": ";
        string* returns = SerialDijkstra(AdjList, numNodes, startNode, endNode);
        cout << "Serial Distance: " << returns[0] << " Serial Time: " << returns[1] << " ";
        delete[] returns;

        returns = ParallelDijkstra(AdjList, numNodes, startNode, endNode, NUM_THREADS);
        cout << "Parallel Distance: " << returns[0] << " Serial Time: " << returns[1] << endl;
        delete[] returns;
    }

    // Clear up the memory
    Node* current;
    Node* next;
    for (int i = 0; i < numNodes; i++) {
        current = AdjList[i];
        while(current != nullptr) {
            next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] AdjList;

    return 1;
}