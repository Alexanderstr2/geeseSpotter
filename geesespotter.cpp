//Goose Spotter Main
/*
Compile:
g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11
*/
#include "geesespotter_lib.h"

char * createBoard(std::size_t xdim, std::size_t ydim){
    //Array Size
    std::size_t array_size{0};
    array_size = xdim*ydim;
    
    char * array { new char [array_size]{0} }; // allocate array

    //Return Pointer
    return array;
}

void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim){
    int total{0};
    total = xdim*ydim;
    for (int k{0}; k<total; ++k) {
        //Calulate row and col
        int row{0};
        int col{0};
        row = (k/xdim) + 1;
        if (k%xdim == 0) {
            row += 1;
        }
        col = k - (row*xdim);

        int goose{0}; //Number of geese
        for (int i{(row - 1)}; i <= (row + 1); ++i) {
            for (int l{(col - 1)}; l <= (col + 1); ++l) {
                if (i < 1) { //Break if not a space
                    break;
                }
                if (l < 1) { //Break if not a space
                    break;
                }
                if ((i == row) && (l == col)) { //Break if center space
                    break;
                }
                int num{0};
                num = (valueMask() & board[(i*xdim)+ l]);
                if (num == 9) { //If goose present add 1 to goose value
                    ++goose;
                }
            }
        }
        //Increase value by amount of geese
        //Set new value in lower 4 bits
        board[k] = (board[k] | goose);
    }
}

void hideBoard(char * board, std::size_t xdim, std::size_t ydim){
    //Hide all the field values

    //Array size
    std::size_t array_size{0};
    array_size = xdim*ydim;

    //For loop
    for (std::size_t i{0}; i < array_size; ++i) {
        board[i] = hiddenBit() | board[i];
    }
}

void cleanBoard(char * board){
    //To deallocate the given board
    delete[] board;
    board = nullptr;

}

void printBoard(char * board, std::size_t xdim, std::size_t ydim){

    std::size_t index {0};
    for (int i{0}; i < ydim; ++i) {
        for (int k{0}; k < xdim; ++k) {
            if ((markedBit() & board[index]) == markedBit()){
                std::cout << "M"; //If marked print M
            } else if ((hiddenBit() & board[index]) == hiddenBit()){
                std::cout << "*"; // If hidden print *
            } else {
                std::cout << (int) board[index]; //Otherwise print value at that point
            }
            ++index;
        }
        std::cout << std::endl;
    }
}


int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){
    if ((hiddenBit() & board[((yloc)*xdim) + xloc]) != hiddenBit()){
         //If unhidden return 2
        return 2;
    } else if ((markedBit() & board[((yloc)*xdim) + xloc]) == markedBit()) {
        return 1;
        //If marked return 1
    } else if ((board[((yloc)*xdim) + xloc] & valueMask()) == 9) {
        //Return 9 if it contains a goose
        board[((yloc)*xdim) + xloc] = (board[((yloc)*xdim) + xloc] & ~hiddenBit());
        return 9;
    } else if ((board[((yloc)*xdim) + xloc] & valueMask()) == 0) {
        //If 0
        int k{0};
        k = ((yloc)*xdim) + xloc;
        int row{0};
        int col{0};
        row = (k/xdim) + 1;
        if (k%xdim == 0) {
            row += 1;
        }
        col = k - (row*xdim);

        for (int i{(row - 1)}; i <= (row + 1); ++i) {
            for (int l{col - 1}; l <= (col + 1); ++l) {
                if (i < 1) { //Break if not a space
                    break;
                }
                if (l < 1) { //Break if not a space
                    break;
                }
                if ((i == row) && (l == col)) { //Break if center space
                    break;
                }
                if ((hiddenBit() & board[(i*xdim) + l]) == hiddenBit()) { //If hidden Break
                    break;
                }
            }
        }
        board[((yloc)*xdim) + xloc] = (board[((yloc)*xdim) + xloc] & ~hiddenBit());
        return 0;

    } else {
        board[((yloc)*xdim) + xloc] = (board[((yloc)*xdim) + xloc] & ~hiddenBit());
        return 0;
    }
}

int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){

    if ((hiddenBit() & board[((yloc)*xdim) + xloc]) != hiddenBit()){
         //If unhidden return 2
        return 2;
    } else if ((markedBit() & board[((yloc)*xdim) + xloc]) == markedBit()) {
        //If marked unmark
        board[((yloc)*xdim) + xloc] = (board[((yloc)*xdim) + xloc] & ~markedBit());
        return 0;
    } else {
        //if unmarked / other mark
        board[((yloc)*xdim) + xloc] = (board[((yloc)*xdim) + xloc] | markedBit());
        return 0;
    }
}

bool isGameWon(char * board, std::size_t xdim, std::size_t ydim){
    //All the hidden feilds are 9
    int total{0};
    total = xdim*ydim;
    for (int k{1}; k<=total; ++k) {
        int n = (valueMask() & board[k] + 49);
        if (((hiddenBit() & board[k]) == hiddenBit()) && (n != 9)) { //All hidden feilds are geese
            return false;
        }
        if (((hiddenBit() & board[k]) != hiddenBit()) && (n == 9)) { //All non hidden feilds do don't contain geese
            return false;
        }
        // At least one turn has been played SEE INSTRUCTIONS
        return true;
    }
    return false;
}