#include <iostream>
#include "types.h"
#include "uci.h"
#include "pvtable.h"
#include "board.h"

using namespace ChessCpp;

int main() {
    Magics::init();
    Board::init_zobrist();

    Board board;
    MoveList list;

    constexpr auto TEST_FEN = "r1bq2r1/b4pk1/p1pp1p2/1p2pP2/1P2P1PB/3P4/1PPQ2P1/R3K2R w 0 1";
    board.set(START_FEN);

    std::string input;
    while (true) {
        board.print();
        std::cin >> input;
        if (input[0] == 'q') {
            break;
        } else if (input[0] == 't') {
            board.undo_move();
        } else if (input[0] == 'p') {
            board.perftTest(5);
        } else {
            Move move = Uci::parseMove(input, board);
            if (move == Move::none()) {
                std::cout << "Illegal move.\n";
                continue;
            }
            board.pvTable.store(board, move);
            board.do_move(move);
        }

    };

    // board.perftTest(5);

    std::cin.get();
    return 0;
}
