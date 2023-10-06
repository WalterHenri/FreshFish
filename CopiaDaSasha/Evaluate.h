#ifndef EVALUATE_HPP_INCLUDED
#define EVALUATE_HPP_INCLUDED

#include <math.h>
#include "Board.h"
#include "Piece.h"
#define IMESUARABLE 1
#define WORST_THING_POSSIBLE 0
#define BEST_THING_POSSIBLE 1
#define ONE_OF_THE_THINGS_POSSIBLE 0.5;


float squareValue = 0.1;

double sigmoid(double grade) {
    return 1 / (1 + pow(2.718281828, -grade));
}

/* gives value to a instant position*/

double evaluatePosition(ChessBoard board);
double bishopValue(ChessBoard board, int square, int pieceColor);
double horseValue(ChessBoard board, int square, int pieceColor);

double horseValue(ChessBoard board, int square, int pieceColor) {





}




double bishopValue(ChessBoard board, int square, int pieceColor) {

    double bishop = 3;


    //the value of the bishop can raise if its is in a good diagonal

    if (pieceColor == PIECE_BLACK) {
        for (int i = square; i < 64; i += 9) {
            int pieceType = PieceGetType(board.squares[i]);
            int Color = PieceGetColor(board.squares[i]);



            if (pieceType == PIECE_PAWN && Color == pieceColor) {
                bishop -= 0.1;
            }
            else if (pieceType == PIECE_PAWN && Color != pieceColor) {
                bishop -= 0.3;
            }
            else if (pieceType == PIECE_KNIGHT && Color != pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KNIGHT && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KING && Color != pieceColor) {
                bishop += 0.5;
            }
            else if (pieceType == PIECE_KING && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop += 0.3;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else {
                bishop += 0.2;
            }

        }

        for (int i = square; i > 0; i -= 9) {
            int pieceType = PieceGetType(board.squares[i]);
            int Color = PieceGetColor(board.squares[i]);


            if (pieceType == PIECE_PAWN && Color == pieceColor) {
                bishop += 0.2;
            }
            else if (pieceType == PIECE_PAWN && Color != pieceColor) {
                bishop += 0.2;
            }
            else if (pieceType == PIECE_KNIGHT && Color != pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KNIGHT && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KING && Color != pieceColor) {
                bishop += 0.5;
            }
            else if (pieceType == PIECE_KING && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop += 0.3;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else {
                bishop += 0.2;
            }

        }

    }
    else if (pieceColor == PIECE_WHITE) {
        for (int i = square; i > 0; i -= 9) {
            int pieceType = PieceGetType(board.squares[i]);
            int Color = PieceGetColor(board.squares[i]);

            if (pieceType == PIECE_PAWN && Color == pieceColor) {
                bishop -= 0.1;
            }
            else if (pieceType == PIECE_PAWN && Color != pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_KNIGHT && Color != pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KNIGHT && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KING && Color != pieceColor) {
                bishop += 0.5;
            }
            else if (pieceType == PIECE_KING && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop += 0.3;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else {
                bishop += 0.2;
            }

        }

        for (int i = square; i < 64; i += 9) {
            int pieceType = PieceGetType(board.squares[i]);
            int Color = PieceGetColor(board.squares[i]);


            if (pieceType == PIECE_PAWN && Color == pieceColor) {
                bishop += 0.2;
            }
            else if (pieceType == PIECE_PAWN && Color != pieceColor) {
                bishop += 0.2;
            }
            else if (pieceType == PIECE_KNIGHT && Color != pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KNIGHT && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_KING && Color != pieceColor) {
                bishop += 0.5;
            }
            else if (pieceType == PIECE_KING && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop += 0.3;
            }
            else if (pieceType == PIECE_QUEEN && Color == pieceColor) {
                bishop -= 0.2;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else if (pieceType == PIECE_BISHOP && Color == pieceColor) {
                bishop += 0.1;
            }
            else {
                bishop += 0.2;
            }

        }
    }

    return (pieceColor == PIECE_BLACK) ? bishop : -bishop;

}
double evaluatePosition(ChessBoard board) {

    double grade = 0;
    int sinal;
    //contando material
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            int index = i * 8 + j;
            int pieceType = PieceGetType(board.squares[index]);
            int pieceColor = PieceGetColor(board.squares[index]);
            sinal = (pieceColor == PIECE_BLACK) ? 1 : -1;


            switch (pieceType) {

            case PIECE_BISHOP:
                grade += bishopValue(board, index, pieceColor);
                break;
            case PIECE_KNIGHT:
                //                grade += horseValue*sinal;
                break;
            case PIECE_PAWN:
                // grade += pawnValue*sinal;
                break;
            case PIECE_QUEEN:
                //  grade += queenValue*sinal;
                break;
            case PIECE_ROOK:
                //  grade += rookValue*sinal;
            default:
                break;

            }
        }
    }

    return sigmoid(grade);
}

#endif // EVALUATE_HPP_INCLUDED
