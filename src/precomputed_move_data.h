#ifndef PRECOMPUTED_MOVE_DATA_H
#define PRECOMPUTED_MOVE_DATA_H

#include <algorithm>
#include "types.h"

namespace ChessCpp::Precomputed {
namespace {
    constexpr int KnightOffsets[8] = {17, 10, -6, -15, -17, -10, 6, 15};
}

inline int NumSquaresToEdge[SQUARE_NB][8];
inline Square KnightMoves[SQUARE_NB][8];

constexpr void init() {
    for (Square sq = SQ_A1; sq < SQUARE_NB; ++sq) {
        Rank rank = rank_of(sq);
        File file = file_of(sq);

        int north = 7 - rank;
        int south = rank;
        int west = file;
        int east = 7 - file;

        NumSquaresToEdge[sq][0] = north;
        NumSquaresToEdge[sq][1] = south;
        NumSquaresToEdge[sq][2] = west;
        NumSquaresToEdge[sq][3] = east;
        NumSquaresToEdge[sq][4] = std::min(north, west);
        NumSquaresToEdge[sq][5] = std::min(south, east);
        NumSquaresToEdge[sq][6] = std::min(north, east);
        NumSquaresToEdge[sq][7] = std::min(south, west);

        for (int i = 0; i < 8; ++i) {
            Square targetSq = sq + Direction(KnightOffsets[i]);
            KnightMoves[sq][i] = SQ_NONE;
            if (is_ok(targetSq)) {
                Rank targetRank = rank_of(targetSq);
                File targetFile = file_of(targetSq);
                if (std::max(std::abs(rank - targetRank), std::abs(file - targetFile)) == 2) {
                    KnightMoves[sq][i] = targetSq;
                }
            }
        }
    }
}
}

#endif  // PRECOMPUTED_MOVE_DATA_H
