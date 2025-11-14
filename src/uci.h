#ifndef UCI_H
#define UCI_H

#include <string>
#include "types.h"
#include "board.h"
#include "movegen.h"

namespace ChessCpp {
namespace Uci {
    // assume valid string
    inline Move parseMove(std::string& str, Board& board) {
        Square from = make_square(File(str[0] - 'a'), Rank(str[1] - '1'));
        Square to = make_square(File(str[2] - 'a'), Rank(str[3] - '1'));

        MoveList list;
        MoveGen::generate_pseudo_moves(board, list);

        for (int i = 0; i < list.count; ++i) {
            Move& move = list.moves[i];
            if (move.from_sq() == from && move.to_sq() == to) {
                if (move.type_of() == PROMOTION) {
                    PieceType type = move.promotion_type();
                    if ((type == KNIGHT && str[4] == 'n') || (type == ROOK && str[4] == 'r')
                        || (type == BISHOP && str[4] == 'b') || (type == QUEEN && str[4] == 'q')) {
                        return move;
                    }
                    continue;
                }
                return move;
            }
        }
        return Move::none();
    }
}  // namespace Uci
}  // namespace ChessCpp

#endif  // UCI_H