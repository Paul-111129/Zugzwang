#include "movegen.h"
#include "bitboard.h"
#include "board.h"
#include "types.h"
#include <iostream>

namespace Zugzwang {
    
void MoveGen::GenerateSlidingMoves(const Board& board, MoveList& list) {
    Color side = board.SideToMove;
    Bitboard occupancy = board.ByColorBB[WHITE] | board.ByColorBB[BLACK];

    static constexpr Piece pieceIdx[][3] = { { W_BISHOP, W_ROOK, W_QUEEN }, { B_BISHOP, B_ROOK, B_QUEEN } };

    for (int i = 0; i < 3; i++) {
        Piece piece = pieceIdx[side][i];
        int pieceNb = board.PieceNumber[piece];

        for (int pieceIdx = 0; pieceIdx < pieceNb; ++pieceIdx) {
            Square startSq = board.PieceList[piece][pieceIdx];

            Bitboard attacks = 0ULL;

            if (i == 0) {
                attacks = Bitboards::GetBishopAttacks(startSq, occupancy);
            } else if (i == 1) {
                attacks = Bitboards::GetRookAttacks(startSq, occupancy);
            } else if (i == 2) {
                attacks = Bitboards::GetQueenAttacks(startSq, occupancy);
            }

            attacks &= ~board.ByColorBB[side];

            while (attacks) {
                Square targetSq = PopLsb(attacks);
                list.Moves[list.Count++] = Move(startSq, targetSq);
            }
        }
    }
}

void MoveGen::GenerateKingMoves(const Board& board, MoveList& list) {
    const Color color = board.SideToMove;
    Square startSq = board.KingSquare[color];

    Bitboard attacks = Bitboards::GetKingAttacks(startSq) & ~board.ByColorBB[color];
    while (attacks) {
        Square targetSq = PopLsb(attacks);
        list.Moves[list.Count++] = Move(startSq, targetSq);
    }

    // castling
    if (color == WHITE) {
        if (board.CastlingRights & WHITE_OO) {
            if (board.Pieces[SQ_F1] == NO_PIECE && board.Pieces[SQ_G1] == NO_PIECE) {
                if (!IsSquareAttacked(board, SQ_E1, BLACK) && !IsSquareAttacked(board, SQ_F1, BLACK)) {
                    list.Moves[list.Count++] = Move::Make<CASTLING>(SQ_E1, SQ_G1);
                }
            }
        }

        if (board.CastlingRights & WHITE_OOO) {
            if (board.Pieces[SQ_D1] == NO_PIECE && board.Pieces[SQ_C1] == NO_PIECE &&
                board.Pieces[SQ_B1] == NO_PIECE) {
                if (!IsSquareAttacked(board, SQ_E1, BLACK) && !IsSquareAttacked(board, SQ_D1, BLACK)) {
                    list.Moves[list.Count++] = Move::Make<CASTLING>(SQ_E1, SQ_C1);
                }
            }
        }
    } else {
        if (board.CastlingRights & BLACK_OO) {
            if (board.Pieces[SQ_F8] == NO_PIECE && board.Pieces[SQ_G8] == NO_PIECE) {
                if (!IsSquareAttacked(board, SQ_E8, WHITE) && !IsSquareAttacked(board, SQ_F8, WHITE)) {
                    list.Moves[list.Count++] = Move::Make<CASTLING>(SQ_E8, SQ_G8);
                }
            }
        }

        if (board.CastlingRights & BLACK_OOO) {
            if (board.Pieces[SQ_D8] == NO_PIECE && board.Pieces[SQ_C8] == NO_PIECE &&
                board.Pieces[SQ_B8] == NO_PIECE) {
                if (!IsSquareAttacked(board, SQ_E8, WHITE) && !IsSquareAttacked(board, SQ_D8, WHITE)) {
                    list.Moves[list.Count++] = Move::Make<CASTLING>(SQ_E8, SQ_C8);
                }
            }
        }
    }
}

void MoveGen::GenerateKnightMoves(const Board& board, MoveList& list) {
    const Color color = board.SideToMove;

    for (int pieceNum = 0; pieceNum < board.PieceNumber[MakePiece(color, KNIGHT)]; ++pieceNum) {
        Square startSq = board.PieceList[MakePiece(color, KNIGHT)][pieceNum];
        Bitboard attacks = Bitboards::GetKnightAttacks(startSq) & ~board.ByColorBB[color];

        while (attacks) {
            Square targetSq = PopLsb(attacks);
            list.Moves[list.Count++] = Move(startSq, targetSq);
        }
    }
}

void MoveGen::GeneratePawnMoves(const Board& board, MoveList& list) {
    const Color color = board.SideToMove;
    const Rank startRank = RelativeRank(color, RANK_2);
    const Rank promoRank = RelativeRank(color, RANK_7);

    for (int pieceIdx = 0; pieceIdx < board.PieceNumber[MakePiece(color, PAWN)]; pieceIdx++) {
        const Square startSq = board.PieceList[MakePiece(color, PAWN)][pieceIdx];
        const Rank rank = RankOf(startSq);
        const Square oneForward = startSq + PawnPush(color);
        ASSERT(IsOk(oneForward));
        // pushes
        if (board.Pieces[oneForward] == NO_PIECE) {
            if (rank == promoRank) {
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, oneForward, KNIGHT);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, oneForward, BISHOP);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, oneForward, ROOK);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, oneForward, QUEEN);
            } else {
                const Square twoForward = startSq + 2 * PawnPush(color);
                ASSERT(IsOk(twoForward));

                list.Moves[list.Count++] = Move(startSq, oneForward);
                if (rank == startRank && board.Pieces[twoForward] == NO_PIECE) {
                    list.Moves[list.Count++] = Move(startSq, twoForward);
                }
            }
        }

        // captures
        Bitboard attacks = Bitboards::GetPawnAttacks(color, startSq) & board.ByColorBB[~color];
        while (attacks) {
            Square to = PopLsb(attacks);
            if (rank == promoRank) {
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, to, KNIGHT);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, to, BISHOP);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, to, ROOK);
                list.Moves[list.Count++] = Move::Make<PROMOTION>(startSq, to, QUEEN);
            } else {
                list.Moves[list.Count++] = Move(startSq, to);
            }
        }

        // en passant
        if (board.EpSquare != SQ_NONE &&
            (Bitboards::GetPawnAttacks(color, startSq) & (1ULL << board.EpSquare))) {
            list.Moves[list.Count++] = Move::Make<EN_PASSANT>(startSq, board.EpSquare);
        }
    }
}

bool MoveGen::IsSquareAttacked(const Board& board, Square sq, Color attacker) {
    Bitboard sqBb = SquareBb(sq);

    for (int i = 0; i < board.PieceNumber[MakePiece(attacker, PAWN)]; ++i) {
        if (Bitboards::GetPawnAttacks(attacker, board.PieceList[MakePiece(attacker, PAWN)][i]) & sqBb) {
            return true;
        }
    }

    for (int i = 0; i < board.PieceNumber[MakePiece(attacker, KNIGHT)]; ++i) {
        if (Bitboards::GetKnightAttacks(board.PieceList[MakePiece(attacker, KNIGHT)][i]) & sqBb) {
            return true;
        }
    }

    if (Bitboards::GetKingAttacks(board.KingSquare[attacker]) & sqBb) {
        return true;
    }

    Bitboard occupied = board.ByColorBB[WHITE] | board.ByColorBB[BLACK];
    for (int i = 0; i < board.PieceNumber[BISHOP + attacker * 8]; ++i) {
        if (Bitboards::GetBishopAttacks(board.PieceList[BISHOP + attacker * 8][i], occupied) & sqBb) {
            return true;
        }
    }

    for (int i = 0; i < board.PieceNumber[ROOK + attacker * 8]; ++i) {
        if (Bitboards::GetRookAttacks(board.PieceList[ROOK + attacker * 8][i], occupied) & sqBb) {
            return true;
        }
    }

    for (int i = 0; i < board.PieceNumber[QUEEN + attacker * 8]; ++i) {
        if (Bitboards::GetQueenAttacks(board.PieceList[QUEEN + attacker * 8][i], occupied) & sqBb) {
            return true;
        }
    }
    return false;
}

void MoveGen::GeneratePseudoMoves(const Board& board, MoveList& list) {
    GeneratePawnMoves(board, list);
    GenerateSlidingMoves(board, list);
    GenerateKnightMoves(board, list);
    GenerateKingMoves(board, list);
}

bool MoveGen::IsMoveLegal(Board& board, const Move move) {
    MoveList list;
    GeneratePseudoMoves(board, list);
    for (int i = 0; i < list.Count; ++i) {
        if (!board.MakeMove(move)) {
            continue;
        }
        board.UnmakeMove();
        if (list.Moves[i] == move) {
            return true;
        }
    }
    return false;
}

} // namespace Zugzwang
