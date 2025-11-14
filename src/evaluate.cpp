#include "evaluate.h"
#include "board.h"

namespace ChessCpp {
int Evaluation::evaluate(const Board& board) {
    int perspective = (board.sideToMove == WHITE ? 1 : -1);
    return (board.material[WHITE] - board.material[BLACK]) * perspective;
}
}  // namespace ChessCpp