#pragma once
// Header file for creation of hex board for Coursera C++
#include <iostream>
#include <vector>

using namespace std;

#include "square.h"

class HexBoard
{
    private:
        vector<vector<Square>> board;           // holds board of Squares
        
    public:
        HexBoard();
        ~HexBoard();
        void initialize(int size);                 // initialize new board
        void displayBoard();                    // display created graph
        void computerMove(const char player, string& move);
                                                // returns move in move
        bool makeMove(const char player, string& move);
                                                // makes move in move
        static bool gameOver;
};