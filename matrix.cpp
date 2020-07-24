#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

#include "square.h"
#include "node.h"
#include "matrix.h"

int Matrix::edgeGraphLength;

// Default constructor for edgeGraph
// edgeGraph will need to be size * size
// to hold all nodes in one-dimensional vector
Matrix::Matrix(int size)
{
    Matrix::edgeGraphLength = size;
    edgeGraph.resize(size * size);
    distIndex.resize(size * size);
    prevIndex.resize(size * size);
}

Matrix::~Matrix()
{
}

void Matrix::copyMatrix(Matrix *m)
{
    for (int i = 0; i < Matrix::edgeGraphLength * edgeGraphLength; i++)
    {
        m->edgeGraph[i].clear();
        for (auto jt = this->edgeGraph[i].begin(); jt != this->edgeGraph[i].end(); ++jt)
        {
            Node *n = new Node(jt->nodeIndex);
            n->length = jt->length;
            m->edgeGraph[i].push_back(*n);
        }
    }   
    m->sourceList.clear();
    m->targetList.clear();    
    for (int i = 0; i < Matrix::edgeGraphLength; i++)
    {
        m->sourceList.push_back(this->sourceList[i]);
        m->targetList.push_back(this->targetList[i]);
    }
}

int Matrix::getNodeIndex(int row, int col)
{
    return row * Matrix::edgeGraphLength + col;
}

void Matrix::getRC(int& row, int& col, int nodeIndex)
{
    row = nodeIndex / Matrix::edgeGraphLength;
    col = nodeIndex % Matrix::edgeGraphLength;
}

// removes connection if it exists from
// Node x to Node y
void Matrix::disconnect(Node x, Node y)
{
    // find y in x's list of outgoing connecting nodes
    for (auto it = edgeGraph[x.nodeIndex].begin(); it != edgeGraph[x.nodeIndex].end(); ++it)
    {
        if (it->nodeIndex == y.nodeIndex)
        {
            it = edgeGraph[x.nodeIndex].erase(it);
        }
    }
    // erase incoming connection, too
    for (auto it = edgeGraph[y.nodeIndex].begin(); it != edgeGraph[y.nodeIndex].end(); ++it)
    {
        if (it->nodeIndex == x.nodeIndex)
        {
            it = edgeGraph[y.nodeIndex].erase(it);
        }
    }
}

// adds connection from Node x to Node y and 
// sets length of connection to length in Node y
// first checks if connection exists and deletes it to
// replace with this new one
void Matrix::connect(Node x, Node y, int length)
{
    Matrix::disconnect(x, y);
    y.length = length;
    edgeGraph[x.nodeIndex].push_back(y);
    // connect symmetric nodes
    x.length = length;
    edgeGraph[y.nodeIndex].push_back(x);
}

int Matrix::getLength(Node x, Node y)
{
    for (auto it = edgeGraph[x.nodeIndex].begin(); it != edgeGraph[x.nodeIndex].end(); ++it)
    {
        if (it->nodeIndex == y.nodeIndex)
        {
            return it->length;
        }
    }
    return INFINITE;
}

bool Matrix::isSource(int nodeIndex)
{    
    return count(sourceList.begin(), sourceList.end(), nodeIndex) > 0;
}

bool Matrix::isTarget(int nodeIndex)
{
    return count(targetList.begin(), targetList.end(), nodeIndex) > 0;
    
}

list<Node> Matrix::getNeighbors(int nodeIndex)
{
    return edgeGraph[nodeIndex];
}

string Matrix::getName(int nodeIndex)
{
    int row, column;
    Matrix::getRC(row, column, nodeIndex);
    return Square::getName(row, column);
}

void Matrix::initializeNewGraph(char player)
{
    // populate connections 
    for (int row = 0; row < Matrix::edgeGraphLength; row++)
    {
        for (int col = 0; col < Matrix::edgeGraphLength; col++)
        {
            if (row > 0)
            {
                Matrix::connect(Node(row, col), Node(row - 1, col), 2);
                if (col < Matrix::edgeGraphLength - 1)
                {
                    Matrix::connect(Node(row, col), Node(row - 1, col + 1), 2);
                }
            }
            if (col > 0)
            {
                Matrix::connect(Node(row, col), Node(row, col - 1), 2);
                if (row < Matrix::edgeGraphLength - 1)
                {
                    Matrix::connect(Node(row, col), Node(row + 1, col - 1), 2);
                }
            }
            if (col < Matrix::edgeGraphLength - 1)
            {
                Matrix::connect(Node(row, col), Node(row, col + 1), 2);
            }
            if (row < Matrix::edgeGraphLength - 1)
            {
                Matrix::connect(Node(row, col), Node(row + 1, col), 2);
            }
        }
    }
    // initialize source and target lists
    if (player == Square::BLACK)
    {
        for (int i = 0; i < edgeGraphLength; i++)
        {
            // black's sources are node indexes for row, col (0, 0), (0, 1), (0, 2) ...
            sourceList.push_back(getNodeIndex(0, i));
            targetList.push_back(getNodeIndex(edgeGraphLength - 1, i));
        }
    }
    else
    {
        for (int i = 0; i< edgeGraphLength; i++)
        {
            // white's sources are node indexes for row, col (0, 0), (1, 0), (2, 0) ...
            sourceList.push_back(getNodeIndex(i, 0));
            targetList.push_back(getNodeIndex(i, edgeGraphLength - 1));
        }
    }
}

// when my piece is added, it means existing connections to square
// are reduced by one (unless they are INFINITE)
void Matrix::addMyPiece(int row, int col)
{
    Node *pn = new Node(row, col);
    // go through all neighbors of n and reduce length of each outgoing connection by 1
    // (unless len is already 0)
    for (auto iNeighbor = edgeGraph[pn->nodeIndex].begin(); iNeighbor != edgeGraph[pn->nodeIndex].end(); ++iNeighbor)
    {
        if (iNeighbor->length > 0)
        {
            iNeighbor->length--;
            // now do the same thing with the (single) incoming connection from neighbor to n
            // which we assume to be > 0 if iNeighbor is, too
            for (auto jNeighbor = edgeGraph[iNeighbor->nodeIndex].begin(); jNeighbor != edgeGraph[iNeighbor->nodeIndex].end(); jNeighbor++)
            {
                if (jNeighbor->nodeIndex == pn->nodeIndex)
                {
                    jNeighbor->length--;
                    break;
                }
            }
        }
    }
}

// when their piece is added, it means there is no longer any connection
// to the square (it is disconnected)
void Matrix::addTheirPiece(int row, int col)
{
    Node *pn = new Node(row, col);
    // while the list of neighbors of n is not empty, disconnect connecting
    // nodes (note this disconects incoming and outgoing connections)
    while (!edgeGraph[pn->nodeIndex].empty())
    {
        Node m = edgeGraph[pn->nodeIndex].front();
        // this shouldn't be nece 
        // edgeGraph[pn->nodeIndex].pop_front();
        Matrix::disconnect(*pn, m);
    }
    
}

void Matrix::displayGraph()
{
    for (int i = 0; i < edgeGraph.size(); i++)
    {
        cout << "node " << Matrix::getName(i) << ", neighbors ";
        list<Node> n = Matrix::getNeighbors(i);
        for (auto it = n.begin(); it != n.end(); ++it)
        {
            cout << getName(it->nodeIndex) << " (len " << it->length << ") ";
        }
        cout << endl;
    }
}

void Matrix::displaySPath(char player)
{

    // find target with smallest distance
    int savesmallest = INFINITE;
    int savetarget = INFINITE;
    for (int i = 0; i < edgeGraphLength; i++)
    {
        if (distIndex[targetList[i]] < savesmallest)
        {
            savetarget = targetList[i];
            savesmallest = distIndex[targetList[i]];
        }
    }
    if (savesmallest == INFINITE)
    {
        cout << "no path to any target for " << player <<", other team won" << endl;
    }
    else
    {
        cout << "shortest path is to target vertex " << getName(savetarget) << endl;
        cout << "path back to source, total distance " << savesmallest << endl;
        int j = savetarget;
        do {
            cout << "    node " << getName(j) << " to node " << getName(prevIndex[j]) << endl;
            j = prevIndex[j];
        } while (!isSource(j));
    }
}


int Matrix::findSPath()
{
    int shortestPath = INFINITE;
    // set all dist and prev to INF
    for (int i = 0; i < edgeGraphLength * edgeGraphLength; i++)
    {
        distIndex[i] = INFINITE;
        prevIndex[i] = INFINITE;
    }
    for (int sourceCount = 0; sourceCount < edgeGraphLength; sourceCount++)
    {
        list<int> OpenList, ClosedList;             // Open holds vertices that haven't been visited
                                                   // Closed that have
        // distance to source is 0
        distIndex[sourceList[sourceCount]] = 0;
        // put initial source on OpenList
        OpenList.push_back(sourceList[sourceCount]);
        // keep examining nodes until OpenList empty
        while (!OpenList.empty())
        {
            // get element in OpenList with smallest dist[]
            int u = OpenList.front();
            for (list<int>::iterator it = next(OpenList.begin(), 1); it != OpenList.end(); ++it)
            {
                if (distIndex[*it] < distIndex[u])
                {
                    u = *it;
                }
            }
            // remove the vertex with that index from Open
            OpenList.remove(u);
            // if we've retrieved target, we're done
            if (isTarget(u))
                {
                    OpenList.clear();
                    if (distIndex[u] < shortestPath)
                    {
                        shortestPath = distIndex[u];
                    }
                    break;                  // leave while loop
                }
            // Otherwise add to closed
            ClosedList.push_back(u);
            // now iterate through neighbors of index
            list<Node> n = edgeGraph[u];
            // pv iterates through neighbors
            for (auto pv = n.begin(); pv != n.end(); ++pv)
            {
                // only do this for nodes not in Closed or Open
                if (count(ClosedList.begin(), ClosedList.end(), pv->nodeIndex) == 0) 
                {
                    int alt = distIndex[u] + pv->length;
                    if (alt < distIndex[pv->nodeIndex])
                    {
                        distIndex[pv->nodeIndex] = alt;
                        prevIndex[pv->nodeIndex] = u;
                    }
                    // add to open if not already on list
                    if (count(OpenList.begin(), OpenList.end(), pv->nodeIndex) == 0)
                    {
                        OpenList.push_back(pv->nodeIndex);
                    }
                }
            }
        }
    }
    return shortestPath;
}