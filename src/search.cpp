#include <algorithm>
#include "search.h"
#include "board.h"
#include "movegen.h"
#include "evaluate.h"

namespace ChessCpp {
int Search::alphaBeta(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) return Evaluation::evaluate(board);

    MoveList list;
    MoveGen::generate_pseudo_moves(board, list);

    bool foundMove = false;

    for (int i = 0; i < list.count; ++i) {
        Move move = list.moves[i];
        if (!board.do_move(move))
            continue;

        foundMove = true;
        int score = -alphaBeta(board, depth - 1, -beta, -alpha);
        board.undo_move(move);

        if (score >= beta)
            return beta;
        alpha = std::max(alpha, score);
    }

    if (!foundMove) {
        // No legal move
        if (board.is_in_check(board.sideToMove))
            return -100000; // checkmate
        else
            return 0; // stalemate
    }

    return alpha;
}
}  // namespace ChessCpp
