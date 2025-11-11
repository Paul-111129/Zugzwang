#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "types.h"

namespace ChessCpp {
// static class
class MoveGen {
  public:
    MoveGen() = delete;
    static void generate_all_moves(MoveList& list);
};
}  // namespace ChessCpp

#endif  // MOVEGEN_H
