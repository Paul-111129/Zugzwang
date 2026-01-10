#pragma once

#include "types.h"

namespace Zugzwang {

class Board;

namespace MoveGen {

bool IsSquareAttacked(const Board& board, Square sq, Color attacker);
void GeneratePseudoMoves(const Board& board, MoveList& list);

} // namespace MoveGen

} // namespace Zugzwang
