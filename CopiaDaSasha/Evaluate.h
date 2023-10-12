#include "MemoEvaluate.h"
#include "math.h"

/*global variables*/

int pawnValue = 1;
int horseValue = 3;
int bishopValue = 3;
int rookValue = 5;
int queenValue = 9;
int kingValue = 1;

float squareValue = 0.1;

/* this function returns a value between zero and one*/
static double sigmoid(double grade) {
    return 1.0 / (1.0 + pow(2.718281828, -grade));
}

double isKingSafe(ChessBoard* board) {

}

double isPieceActive(ChessBoard* board) {

}

double evaluatePosition(ChessBoard* board) {


    struct MemoEvaluation* m = search(boardToKey(board));
    if (m != NULL)
        return m->grade;
    
    double grade = 0;
    int sinal;

    //contando material
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            int pieceType = PieceGetType(board->squares[rank * 8 + file]);
            int pieceColor = PieceGetColor(board->squares[rank * 8 + file]);
            sinal = (pieceColor == PIECE_WHITE) ? 1 : -1;

            switch (pieceType) {
            case PIECE_QUEEN:
                grade += queenValue * sinal;
                break;
            case PIECE_BISHOP:
                grade += bishopValue * sinal;
                break;
            case PIECE_KNIGHT:
                grade += horseValue * sinal;
                break;
            case PIECE_ROOK:
                grade += rookValue * sinal;
                break;
            case PIECE_PAWN:
                grade += pawnValue * sinal;
                break;
            default:
                break;

            }
        }
    }

    // Counting squares of enemies attacks
    double attackSum = 0;
    for (int i = 0; i < 64; i++) {
        if (board->move.attackSquares[i] == true) {
            attackSum += squareValue;
        }
    }

    if (board->state.whoMoves == PIECE_WHITE) {
        grade -= attackSum;
    }
    else {
        grade += attackSum;
    }

    // E se a gente usar INT ao invés de double?
    // 
    grade = sigmoid(grade);
    insert(boardToKey(board), grade);
    return grade;
}