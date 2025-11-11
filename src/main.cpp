#include <iostream>
#include <string>
#include "types.h"
#include "board.h"

using namespace ChessCpp;

int main() {
    const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board board;
    board.set(START_FEN);
    board.print();
    
    std::cin.get();
    return 0;
}
