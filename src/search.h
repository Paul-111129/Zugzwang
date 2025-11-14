#ifndef SEARCH_H
#define SEARCH_H

namespace ChessCpp {
class Board;
namespace Search {
    int alphaBeta(Board& board, int depth, int alpha, int beta);
}  // namespace Search

}  // namespace ChessCpp

#endif  // SEARCH_H