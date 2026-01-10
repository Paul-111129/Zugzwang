#include "board.h"
#include "movegen.h"
#include "types.h"
#include <iostream>

namespace Zugzwang {

static bool IsMoveStr(const std::string& str) {
    auto IsFileValid = [](char ch) { return ch >= 'a' && ch <= 'h'; };
    auto IsRankValid = [](char ch) { return ch >= '1' && ch <= '8'; };
    auto IsPromoValid = [](char ch) { return ch == 'q' || ch == 'r' || ch == 'b' || ch == 'n'; };

    if (str.size() != 4 && str.size() != 5) {
        return false;
    }
    if (!IsFileValid(str[0]) || !IsRankValid(str[1]) || !IsFileValid(str[2]) || !IsRankValid(str[3])) {
        return false;
    }
    if (str[0] == str[2] && str[1] == str[3]) { // same from and to square
        return false;
    }
    if (str.size() == 5 && !IsPromoValid(str[4])) {
        return false;
    }
    return true;
}

static Move ParseMove(std::string_view str, Board& board) {
    Square from = MakeSquare(File(str[0] - 'a'), Rank(str[1] - '1'));
    Square to = MakeSquare(File(str[2] - 'a'), Rank(str[3] - '1'));

    MoveList list;
    MoveGen::GeneratePseudoMoves(board, list);

    for (const auto move : list) {
        if (move.FromSq() == from && move.ToSq() == to) {
            if (move.TypeOf() == PROMOTION) {
                PieceType type = move.PromotionType();
                if ((type == KNIGHT && str[4] == 'n') || (type == ROOK && str[4] == 'r') ||
                    (type == BISHOP && str[4] == 'b') || (type == QUEEN && str[4] == 'q')) {
                    return move;
                }
                continue;
            }
            return move;
        }
    }
    return Move::None();
}

} // namespace Zugzwang

int main() {
    using namespace Zugzwang;

    Bitboards::init();

    Board board;
    MoveList list;

    const char* TEST_FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

    board.ParseFen(TEST_FEN);

    std::string input;

    board.Print();
    while (true) {
        do {
            std::cout << "> ";
            std::getline(std::cin, input);
        } while (input.empty());

        if (input[0] == 'q') {
            break;
        } else if (input[0] == 't') {
            // board.UnmakeMove(move);
            board.Print();
        } else if (input[0] == 'p') {
            board.PerftTest(5);
        } else if (IsMoveStr(input)) {
            Move move = ParseMove(input, board);
            if (move == Move::None()) {
                std::cout << "Illegal move\n";
                continue;
            }
            board.MakeMove(move);
            board.Print();
        } else {
            std::cout << "This is not a move\n";
        }
    };

    return 0;
}
