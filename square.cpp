#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "square.h"

Square::Square()
{
}

Square::~Square()
{
}

// set contents of square to player's piece 
// Square::BLACK, Square::WHITE, or Square::EMPTY
void Square::setContents(const char piece)
{
    this->contents = piece;
}

char Square::getContents()
{
    return this->contents;
}

// returns name e.g. A5, B6)
string Square::getName(int row, int col)
{
    stringstream buffer;
    buffer << (char) (col + 'A') << row + 1;
    return buffer.str();
}

// get row column reference from name
// if error sets them to -1
void Square::getRC(const string& name, int& row, int& col)
{
    try
    {
        col = toupper(name[0]) - 'A';
        row = stoi(name.substr(1,string::npos)) - 1;
    }
    catch (...)
    {
        col = row = -1;
    }
}