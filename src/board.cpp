#include <iostream>
#include <algorithm>
#include <sstream>
#include <cASSERT>
#include <string>
#include <cctype>

#include "board.h"
#include "types.h"

namespace ChessCpp {
namespace {
    constexpr std::string_view PieceToChar(" PNBRQK  pnbrqk");
}

void Board::reset() {
    for (Square i = SQ_A1; i < SQUARE_NB; ++i) {
        pieces[i] = NO_PIECE;
    }

    sideToMove = WHITE;
    epSquare = SQ_NONE;
    rule50 = 0;
    gamePly = 0;
    castlingRights = NO_CASTLING;
}

void Board::set(const std::string& fenStr) {
    reset();

    unsigned char col, row, token;
    size_t idx;
    Square sq = SQ_A8;
    std::istringstream ss(fenStr);

    ss >> std::noskipws;

    // 1. Piece placement
    while ((ss >> token) && !isspace(token)) {
        if (isdigit(token)) {
            sq += (token - '0') * EAST;
        } else if (token == '/') {
            sq += 2 * SOUTH;
        } else if ((idx = PieceToChar.find(token)) != std::string::npos) {
            pieces[sq] = Piece(idx);
            ++sq;
        }
    }

    // 2. Active color
    ss >> token;
    sideToMove = (token == 'w' ? WHITE : BLACK);
    ss >> token;

    // 3. Castling availability
    while ((ss >> token) && !isspace(token)) {
        switch (token) {
        case 'K': castlingRights |= WHITE_OO; break;
        case 'k': castlingRights |= BLACK_OO; break;
        case 'Q': castlingRights |= WHITE_OOO; break;
        case 'q': castlingRights |= BLACK_OOO; break;
        }
    }

    // 4. En passant square
    if (((ss >> col) && (col >= 'a' && col <= 'h'))
        && ((ss >> row) && (row == (sideToMove == WHITE ? '6' : '3')))) {
        epSquare = make_square(File(col - 'a'), Rank(row - '1'));
    }

    // 5. Halfmove clock (rule50)
    ss >> std::skipws >> rule50 >> gamePly;

    // Convert from fullmove starting from 1 to internal ply count
    gamePly = std::max(2 * (gamePly - 1), 0) + (sideToMove == BLACK);
}

void Board::print() const {
    using std::cout;

    cout << "+---+---+---+---+---+---+---+---+\n";
    for (Rank rank = RANK_8; rank >= RANK_1; --rank) {
        for (File file = FILE_A; file <= FILE_H; ++file) {  // <= not <
            Square sq = make_square(file, rank);
            Piece p = pieces[sq];
            char c = (p != NO_PIECE ? PieceToChar[p] : ' ');
            cout << "| " << c << " ";
        }
        cout << "| " << (int(rank) + 1) << "\n"
             << "+---+---+---+---+---+---+---+---+\n";

        if (rank == RANK_1)
            break;  // prevent underflow since rank is unsigned
    }

    cout << "  a   b   c   d   e   f   g   h\n";
    cout << "Side to move: " << (sideToMove == WHITE ? "w" : "b") << "\n";

    cout << "En passant square: ";
    if (is_ok(epSquare))
        cout << epSquare;
    else
        cout << "none";
    cout << "\n";

    cout << "Castle permissions: " << (castlingRights & WHITE_OO ? "K" : "-")
         << (castlingRights & WHITE_OOO ? "Q" : "-") << (castlingRights & BLACK_OO ? "k" : "-")
         << (castlingRights & BLACK_OOO ? "q" : "-") << "\n";
}
}  // namespace ChessCpp
