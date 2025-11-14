#include <iostream>
#include "types.h"
#include "search.h"
#include "uci.h"
#include "board.h"

using namespace ChessCpp;

int main() {
    Magics::init();
    Board::init_zobrist();

    Board board;
    MoveList list;

    constexpr auto TEST_FEN = "r1bq2r1/b4pk1/p1pp1p2/1p2pP2/1P2P1PB/3P4/1PPQ2P1/R3K2R w 0 1";
    board.set(TEST_FEN);

    std::string input;
    while (true) {
        board.print();
        std::cin >> input;
        Move move = Uci::parseMove(input, board);
        board.do_move(move);
        if (board.is_repetition()) {
            std::cout << "Repetition!\n";
        }
    };

    // board.perftTest(5);

    std::cin.get();
    return 0;
}
