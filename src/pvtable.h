#ifndef PVTABLE_H
#define PVTABLE_H

#include "types.h"

namespace ChessCpp {
class Board;

struct PVNode {
    Key posKey;
    Move move;
};

struct PVTable {
    static constexpr int PVSize = 0x100000 * 2;

    PVNode* pvTable = nullptr;
    int numNodes = 0;

    PVTable() {
        numNodes = PVSize / sizeof(PVNode);
        pvTable = new PVNode[numNodes];
        clear_table();
        std::cout << "PvTable init complete with " << numNodes << " nodes\n";
    }

    ~PVTable() { delete[] pvTable; }

    void clear_table();
    void store(const Board& board, const Move& move);
    Move probe(const Board& board);

    int get_pv_line(Board& board, const int depth);
};
}  // namespace ChessCpp

#endif  // PVTABLE_H