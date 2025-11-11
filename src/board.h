#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "types.h"

namespace ChessCpp {
class Board {
  public:
    Piece pieces[SQUARE_NB];
    Color sideToMove;
    Square epSquare;
    int rule50;
    int gamePly;
    int castlingRights;

    void reset();
    void set(const std::string& fenStr);
    void print() const;
};
}  // namespace ChessCpp

#endif  // BOARD_H
