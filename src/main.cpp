#include <iostream>
#include <string>
#include "types.h"
#include "board.h"
#include "movegen.h"
#include "precomputed_move_data.h"

using namespace ChessCpp;

int main() {
    Precomputed::init();
    const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/3r4/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string TEST_FEN = "8/8/8/Pp6/8/8/8/8 w - b6 0 1";

    Board board;
    MoveList list;
    board.set(TEST_FEN);
    board.print();

    MoveGen::generate_all_moves(board, list);
    std::cout << "\n" << list;
    
    std::cin.get();
    return 0;
}
