#ifndef EVALUATE_H
#define EVALUATE_H

namespace ChessCpp {
class Board;
namespace Evaluation {
    int evaluate(const Board& board);
}  // namespace Evaluation

}  // namespace ChessCpp

#endif  // EVALUATE_H