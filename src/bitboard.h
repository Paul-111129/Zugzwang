#pragma once

#include "types.h"

namespace Zugzwang {

class Bitboards {
  public:
    static void Init();

    static Bitboard GetRookAttacks(Square square, Bitboard occupancy) {
        return RookAttackTable[square][((occupancy & RookMasks[square]) * RookMagics[square])
                                       >> RookShifts[square]];
    }
    static Bitboard GetBishopAttacks(Square square, Bitboard occupancy) {
        return BishopAttackTable[square][((occupancy & BishopMasks[square]) * BishopMagics[square])
                                         >> BishopShifts[square]];
    }
    static Bitboard GetQueenAttacks(Square square, Bitboard occupancy) {
        return GetRookAttacks(square, occupancy) | GetBishopAttacks(square, occupancy);
    }

    static Bitboard GetKnightAttacks(Square square) { return KnightAttacks[square]; }
    static Bitboard GetKingAttacks(Square square) { return KingAttacks[square]; }
    static Bitboard GetPawnAttacks(Color color, Square square) {
        return PawnAttacks[color][square];
    }

  private:
    Bitboards() {}

    static const int RookShifts[SQUARE_NB];
    static const int BishopShifts[SQUARE_NB];
    static const Bitboard RookMagics[SQUARE_NB];
    static const Bitboard BishopMagics[SQUARE_NB];
    static const Bitboard RookMasks[SQUARE_NB];
    static const Bitboard BishopMasks[SQUARE_NB];

    static Bitboard RookAttackTable[SQUARE_NB][4096];
    static Bitboard BishopAttackTable[SQUARE_NB][1024];

    static const Bitboard KnightAttacks[64];
    static const Bitboard KingAttacks[64];
    static const Bitboard PawnAttacks[2][64];

    static int CreateBlockerBitboards(Bitboard movementMask, Bitboard blockers[]);
    static Bitboard CreateRookBitboard(Square square, Bitboard blockBitboard);
    static Bitboard CreateBishopBitboard(Square square, Bitboard blockBitboard);
};

constexpr Bitboard SquareBb(Square s) {
    ASSERT(IsOk(s));
    return (1ULL << s);
}

inline Square PopLsb(Bitboard& b) {
    ASSERT(b);
    Square index = Square(__builtin_ctzll(b));
    b &= (b - 1);
    return index;
}

inline Square PopCount(Bitboard b) {
    return Square(__builtin_popcountll(b));
}

inline void SetBit(Bitboard& b, Square sq) {
    b |= (1ULL << sq);
}

inline void ClearBit(Bitboard& b, Square sq) {
    b &= ~(1ULL << sq);
}
} // namespace Zugzwang
