#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

#include "square.h"
#include "hexboard.h"
#include "node.h"
#include "matrix.h"

HexBoard myBoard;

int main()
{
    cout << "Welcome to Hex" << endl;
    
    int bsize = 0;
    int sp = Matrix::INFINITE;
    while (bsize < 1 || bsize > 26)
    {
        cout << "Size of board to generate (1-26): ";
        cin >> bsize;
    }
    myBoard.initialize(bsize);
    cout << "Okay, board and player graphs of size " << bsize << " generated" << endl;
    myBoard.displayBoard();
    cout << "You're White and move first. Computer is Black" << endl;
    while (!HexBoard::gameOver)
    {
        string move;
        bool okay = false;
        while (!okay)
        {
            cout << "Your move: ";
            cin >> move;
            okay = myBoard.makeMove(Square::WHITE, move);
        }
        if (HexBoard::gameOver)
        {
            cout << "white wins" << endl;
            break;
        }
        myBoard.computerMove(Square::BLACK, move);
        if (HexBoard::gameOver)
        {
            cout << "black wins" << endl;
            break;
        }
        myBoard.displayBoard();
    }    
    myBoard.displayBoard();
    cout << "Thank you for playing" << endl;
    return 0;
}