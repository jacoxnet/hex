#pragma once
#include <vector>
#include <list>
#include <string>

using namespace std;

#include "node.h"

class Matrix
{
    private:
        friend class Node;
        vector<list<Node>> edgeGraph;       // holds connection graphs for players w and b
        vector<int> distIndex;      // holds distances from source to node
        vector<int> prevIndex;      // parent of node on shortest path
        vector<int> sourceList, targetList;
                                     // source and target lists for players
        void disconnect(Node x, Node y);
        void connect(Node x, Node y, int length);
        int getLength(Node x, Node y);
        void adjLength(Node x, Node y, int lenAdj); // here, lenAdj is the increm/decrement
        bool isSource(int NodeIndex);
        bool isTarget(int NodeIndex);
        list<Node> getNeighbors(int nodeIndex);
        static int edgeGraphLength;
        static int getNodeIndex(int row, int col);
        static void getRC(int& row, int& col, int nodeIndex);
        static string getName(int nodeIndex);

    public:
        Matrix(int size);
        ~Matrix();
        void copyMatrix(Matrix *m);            // copies to new matrix m
        void initializeNewGraph(char player);     // sets up initial connections on blank hexboard
                                                // and init source list
        void addMyPiece(int row, int col);       // adjust connections as a result of my new piece
        void addTheirPiece(int row, int col);    // adjust connections from their new piece
        void displayGraph();                // display edge graph
        int findSPath();                  // find shortest path from any source to a target
                                            // by repeatedly applying Dijkstra algorithm
                                            // returns length of it
        void displaySPath(char player);     // display info about shortest path found
        static const int INFINITE = 0x3F3F3F3F;
};