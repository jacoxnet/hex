#pragma once
// Header file for square class
// Squares are the individual board pieces
// They are referenced by "name" column ('A' - 'Z') and row (1-based)
// As well as by row, column (0-based)

using namespace std;

class Square
{
    private:
        char contents;                // holds empty, black or white
        
    public:
        static const char EMPTY = 'E';
        static const char BLACK = 'B';
        static const char WHITE = 'W';
        Square();
        ~Square();
        char getContents();           // returns one of the constants
        void setContents(const char piece);
        static string getName(int row, int col);
                                        // converts row, col to A4
        static void getRC(const string& name, int& row, int& col);
                                        // converts A4 to row, col (passed by ref)

                                        // set contents to one of these constants
};