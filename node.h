#pragma once

using namespace std;

class Node
{
    public:
        int nodeIndex;
        int length;
        int totalDistance;
        Node();
        ~Node();
        Node(int nodeIndex);
        Node(int row, int col);
        /*
        bool operator==(const Node& n) const { return this->nodeIndex == n.nodeIndex; }
        bool operator!=(const Node& n) const { return !operator==(n); }
        */
}; 