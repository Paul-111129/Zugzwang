#include "pvtable.h"
#include "movegen.h"
#include "board.h"

namespace ChessCpp {
void PVTable::clear_table() {
    for (PVNode* node = pvTable; node < pvTable + numNodes; ++node) {
        node->posKey = 0ULL;
        node->move = Move::none();
    }
}

void PVTable::store(const Board& board, const Move& move) {
    int i = board.posKey % numNodes;
    ASSERT(i >= 0 && i < numNodes);
    pvTable[i].move = move;
    pvTable[i].posKey = board.posKey;
}

Move PVTable::probe(const Board& board) {
    int i = board.posKey % numNodes;
    ASSERT(i >= 0 && i < numNodes);
    if (pvTable[i].posKey == board.posKey) {
        return pvTable[i].move;
    }
    return Move::none();
}

int PVTable::get_pv_line(Board& board, const int depth) {
    ASSERT(depth < MAX_DEPTH);

    Move move = probe(board);
    int count = 0;

    while (move != Move::none() && count < depth) {
        if (MoveGen::is_move_legal(board, move)) {
            board.do_move(move);
            board.pvArray[count] = move;
        } else {
            break;
        }
        move = probe(board);
    }

    for (int i = count - 1; i >= 0; --i) {
        board.undo_move();
    }

    return count;
}

}  // namespace ChessCpp
