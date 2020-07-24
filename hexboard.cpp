#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

#include "square.h"
#include "hexboard.h"
#include "matrix.h"

Matrix *whiteGraph, *blackGraph;

// Default constructor & destructor
HexBoard::HexBoard()
{
}

HexBoard::~HexBoard()
{
}

bool HexBoard::gameOver;

void HexBoard::initialize(int size)
{
    board.resize(size, vector<Square> (size));

    // set all to EMPTY
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            board[row][col].setContents(Square::EMPTY);
        }
    }
    whiteGraph = new Matrix(size);
    blackGraph = new Matrix(size);
    whiteGraph->initializeNewGraph(Square::WHITE);
    blackGraph->initializeNewGraph(Square::BLACK);
    HexBoard::gameOver = false;
}

// make player move - move is a string of form D3 (col D row 3)
// returns true if valid and move is made else false
bool HexBoard::makeMove(const char player, string& move)
{
    int row, col;
    // convert to row, column
    Square::getRC(move, row, col);
    // check if valid
    if (row == -1)
    {
        return false;
    }
    // retrieve current value
    char current = board[row][col].getContents();
    if (current != Square::EMPTY)
    {
        return false;
    }
    // square is empty so make play
    board[row][col].setContents(player);
    whiteGraph->addMyPiece(row, col);
    blackGraph->addTheirPiece(row, col);       
    cout << "Player move (" << player << ") " << move << endl;
    int sp = whiteGraph->findSPath();
    // whiteGraph->displaySPath(Square::WHITE); 
    cout << "  (shortest path is now " << sp << ")" << endl;
    if (sp == 0)
    {
        HexBoard:gameOver = true;
    }
    return true;
}

void HexBoard::computerMove(const char player, string& move)
{
    Matrix *testM;
    testM = new Matrix(board.size());
    int row, col;
    int score = Matrix::INFINITE;
    // go through whole board. If empty calc shortest path
    // and choose that as move
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board.size(); c++)
        {
            if (board[r][c].getContents() == Square::EMPTY)
            {
                blackGraph->copyMatrix(testM);
                testM->addMyPiece(r, c);
                int s = testM->findSPath();
                if (s < score)
                {
                    score = s;
                    row = r;
                    col = c;
                }
            }
        }
    }
    board[row][col].setContents(player);
    blackGraph->addMyPiece(row, col);
    whiteGraph->addTheirPiece(row, col);
    move = Square::getName(row, col);
    cout << "Computer move (" << player << ") " << move << endl;
    // blackGraph->displaySPath(Square::BLACK);
    score = blackGraph->findSPath();
    cout << "  (shortest path is now " << score << ")" << endl;
    if (score == 0)
    {
        HexBoard:gameOver = true;
    }
}

void HexBoard::displayBoard()
{
    // col header
    cout << "   ";
    for (int head = 0; head < board.size(); head++)
    {
        cout << (char) (65 + head) << "   ";
    }
    cout << "\n";
    for (int row = 0; row < board.size(); row++)
    {
        // these are the two strings we'll build up 
        // for printing each row - 1 line of nodes 1 of links
        string nodeLine = "";
        string linkLine = "";
        // indent to start lines
        nodeLine.string::append(2*row, ' ');
        linkLine.string::append(2*row + 4, ' ');
        // row header - (2 for num plus space)
        string s = to_string(row + 1);
        nodeLine.string::append(2-s.string::length(), ' ').string::append(s).string::append(" ");
        for (int col = 0; col < board[row].size(); col++)
        {
            if (col > 0)
            {
                nodeLine.append(" - ");
                linkLine.string::append("\\ / ");
            }
            char content = board[row][col].getContents();
            if (content == Square::EMPTY)
            {
                nodeLine.string::append(".");
            }
            else if (content == Square::BLACK)
            {
                nodeLine.string::append("B");
            }
            else 
            {
                nodeLine.string::append("W");
            }   
        }
        cout << nodeLine << '\n';
        if (row + 1 < board.size())           // don't print last line of links if at end
        {
            linkLine.string::append("\\");         // need a final link at end
            cout << linkLine << '\n';
        }
    }
}