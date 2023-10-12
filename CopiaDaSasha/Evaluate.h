#include "MemoEvaluate.h"
#include "math.h"

/*global variables*/

#define WORST_THING_POSSIBLE (INT_MIN+100)
#define ONE_OF_THE_THINGS_POSSIBLE 0;
#define BEST_THING_POSSIBLE (INT_MAX-100)


#define PAWNVALUE 100
#define KNIGHTVALUE 300
#define BISHOPVALUE 300
#define ROOKVALUE 500
#define QUEENVALUE 900

#define ATTACKSQUAREVALUE 5

/* this function returns a value between zero and one*/
static double sigmoid(double grade) {
    return 1.0 / (1.0 + pow(2.718281828, -grade));
}

double isKingSafe(ChessBoard* board) {

}

double isPieceActive(ChessBoard* board) {

}

int evaluatePosition(ChessBoard* board) {

    
    struct MemoEvaluation* m = search(boardToKey(board));
    if (m != NULL) {
        //printf("Economizando! Nota: %d\n", m->grade);
        return m->grade;
    }

    //double grade = 0;
    int grade = 0;
    int sinal;

    //contando material
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            int pieceType = PieceGetType(board->squares[rank * 8 + file]);
            int pieceColor = PieceGetColor(board->squares[rank * 8 + file]);
            sinal = (pieceColor == PIECE_WHITE) ? 1 : -1;

            switch (pieceType) {
            case PIECE_QUEEN:
                grade += QUEENVALUE * sinal;
                break;
            case PIECE_BISHOP:
                grade += BISHOPVALUE * sinal;
                break;
            case PIECE_KNIGHT:
                grade += KNIGHTVALUE * sinal;
                break;
            case PIECE_ROOK:
                grade += ROOKVALUE * sinal;
                break;
            case PIECE_PAWN:
                grade += PAWNVALUE * sinal;
                break;
            default:
                break;

            }
        }
    }

    // Counting squares of enemies attacks
    int attackSum = 0;
    for (int i = 0; i < 64; i++) {
        if (board->move.attackSquares[i] == true) {
            attackSum += ATTACKSQUAREVALUE;
        }
    }

    if (board->state.whoMoves == PIECE_WHITE) {
        grade -= attackSum;
    }
    else {
        grade += attackSum;
    }

    // E se a gente usar INT ao invés de double?
    // E QUE TAL AGORA?

    insert(boardToKey(board), grade);
    return grade;
}