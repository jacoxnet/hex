#include <vector>
#include <list>

using namespace std;

#include "matrix.h"

Node::Node()
{
}

Node::Node(int row, int col)
    : nodeIndex(Matrix::getNodeIndex(row, col))
{
}

Node::Node(int nodeIndex)
    : nodeIndex(nodeIndex)
{
}

Node::~Node()
{
}