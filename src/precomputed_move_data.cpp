#include <algorithm>
#include "types.h"

namespace ChessCpp {
int NumSquaresToEdge[SQUARE_NB][8];

constexpr void init() {
    for (Square sq = SQ_A1; sq < SQUARE_NB; ++sq) {
        int north = 7 - rank_of(sq);
        int south = rank_of(sq);
        int west = file_of(sq);
        int east = 7 - file_of(sq);

        NumSquaresToEdge[sq][0] = north;
        NumSquaresToEdge[sq][1] = south;
        NumSquaresToEdge[sq][2] = west;
        NumSquaresToEdge[sq][3] = east;
        NumSquaresToEdge[sq][4] = std::min(north, west);
        NumSquaresToEdge[sq][5] = std::min(south, east);
        NumSquaresToEdge[sq][6] = std::min(north, east);
        NumSquaresToEdge[sq][7] = std::min(south, west);
    }
}
}  // namespace ChessCpp
