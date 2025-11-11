#include <iostream>
#include "types.h"
#include "board.h"
#include "precomputed_move_data.h"
#include "movegen.h"

namespace ChessCpp {
namespace {
    constexpr Direction DirectionOffsets[8] = {NORTH,      SOUTH,      WEST,       EAST,
                                               NORTH_WEST, SOUTH_EAST, NORTH_EAST, SOUTH_WEST};
    constexpr int PawnAttackDirs[2][2] = {
        {4, 6},
        {5, 7}
    };
}

void MoveGen::generate_sliding_moves(const Board& board, Square startSq, MoveList& list) {
    const Piece piece = board.pieces[startSq];
    const int startDirIdx = (type_of(piece) == BISHOP ? 4 : 0);
    const int endDirIdx = (type_of(piece) == ROOK ? 4 : 8);

    for (int dirIdx = startDirIdx; dirIdx < endDirIdx; ++dirIdx) {
        for (int n = 0; n < Precomputed::NumSquaresToEdge[startSq][dirIdx]; n++) {
            Square targetSq = startSq + (n + 1) * DirectionOffsets[dirIdx];
            Piece targetPiece = board.pieces[targetSq];
            ASSERT(is_ok(targetSq));

            if (targetPiece != NO_PIECE && color_of(targetPiece) == board.sideToMove) {
                break;
            }
            list.moves[list.count++] = Move(startSq, targetSq);
            if (targetPiece != NO_PIECE && color_of(targetPiece) == ~board.sideToMove) {
                break;
            }
        }
    }
}

void MoveGen::generate_king_moves(const Board& board, Square startSq, MoveList& list) {
    for (int dirIdx = 0; dirIdx < 8; ++dirIdx) {
        if (Precomputed::NumSquaresToEdge[startSq][dirIdx] > 0) {
            Square targetSq = startSq + DirectionOffsets[dirIdx];
            Piece targetPiece = board.pieces[targetSq];
            ASSERT(is_ok(targetSq));

            if (targetPiece == NO_PIECE || color_of(targetPiece) != board.sideToMove) {
                list.moves[list.count++] = Move(startSq, targetSq);
            }
        }
    }
}

void MoveGen::generate_knight_moves(const Board& board, Square startSq, MoveList& list) {
    for (int knightMoveIdx = 0; knightMoveIdx < 8; ++knightMoveIdx) {
        Square targetSq = Precomputed::KnightMoves[startSq][knightMoveIdx];

        if (targetSq == SQ_NONE)
            continue;  // skip off-board

        Piece targetPiece = board.pieces[targetSq];
        if (targetPiece == NO_PIECE || color_of(targetPiece) != board.sideToMove) {
            list.moves[list.count++] = Move(startSq, targetSq);
        }
    }
}

void MoveGen::generate_pawn_moves(const Board& board, Square startSq, MoveList& list) {
    const Piece piece = board.pieces[startSq];
    const Color color = color_of(piece);
    const Rank rank = rank_of(startSq);
    const Rank startRank = relative_rank(color, RANK_2);
    const Rank promoRank = relative_rank(color, RANK_7);
    const Square oneForward = startSq + pawn_push(color);
    const Square twoForward = startSq + 2 * pawn_push(color);

    if (board.pieces[oneForward] == NO_PIECE) {
        if (rank == promoRank) {
            list.moves[list.count++] = Move::make<PROMOTION>(startSq, oneForward, KNIGHT);
            list.moves[list.count++] = Move::make<PROMOTION>(startSq, oneForward, BISHOP);
            list.moves[list.count++] = Move::make<PROMOTION>(startSq, oneForward, ROOK);
            list.moves[list.count++] = Move::make<PROMOTION>(startSq, oneForward, QUEEN);
        } else {
            list.moves[list.count++] = Move(startSq, oneForward);
            if (rank == startRank && board.pieces[twoForward] == NO_PIECE) {
                list.moves[list.count++] = Move(startSq, twoForward);
            }
        }
    }

    for (int j = 0; j < 2; ++j) {
        if (Precomputed::NumSquaresToEdge[startSq][PawnAttackDirs[color][j]] > 0) {
            Square targetSq = startSq + DirectionOffsets[PawnAttackDirs[color][j]];
            Piece targetPiece = board.pieces[targetSq];

            if (targetPiece != NO_PIECE && color_of(targetPiece) == ~color) {
                if (rank == promoRank) {
                    list.moves[list.count++] = Move::make<PROMOTION>(startSq, targetSq, KNIGHT);
                    list.moves[list.count++] = Move::make<PROMOTION>(startSq, targetSq, BISHOP);
                    list.moves[list.count++] = Move::make<PROMOTION>(startSq, targetSq, ROOK);
                    list.moves[list.count++] = Move::make<PROMOTION>(startSq, targetSq, QUEEN);
                } else {
                    list.moves[list.count++] = Move(startSq, targetSq);
                }
            }

            // en passant
            if (targetSq == board.epSquare) {
                list.moves[list.count++] = Move::make<EN_PASSANT>(startSq, targetSq);
            }
        }
    }
}

void MoveGen::generate_all_moves(const Board& board, MoveList& list) {
    for (Square sq = SQ_A1; sq < SQ_H8; ++sq) {
        Piece piece = board.pieces[sq];
        if (piece == NO_PIECE) {
            continue;
        }
        if (color_of(piece) == board.sideToMove) {
            PieceType type = type_of(piece);
            if (type == ROOK || type == BISHOP || type == QUEEN) {
                generate_sliding_moves(board, sq, list);
            } else if (type == KING) {
                generate_king_moves(board, sq, list);
            } else if (type == KNIGHT) {
                generate_knight_moves(board, sq, list);
            } else if (type == PAWN) {
                generate_pawn_moves(board, sq, list);
            } else {
                ASSERT(false);
            }
        }
    }
}
}  // namespace ChessCpp
