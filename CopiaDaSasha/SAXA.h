#pragma once
#ifndef SAXA_HPP_INCLUDED
#define SAXA_HPP_INCLUDED

/**            SAXA

 Sistema Artificial de Xadrez Academico.
 The greatest chess engine of all time,

 XUXA built in,

 **/

 /* a bot that knows how to play chess.*/

#include <ctype.h>
#include <string.h>
#include <math.h>

#include "Board.h"
#include "Piece.h"

#define IMESUARABLE 1
#define WORST_THING_POSSIBLE 0
#define BEST_THING_POSSIBLE 1

/*global variables*/

int pawnValue = 1;
int horseValue = 3;
int bishopValue = 3;
int rookValue = 5;
int queenValue = 9;
int kingValue = IMESUARABLE;

float squareValue = 0.1;

int saxaDepth; /// warning!!!

int saxaColor;
int saxaOpositeColor;

typedef struct {
    double grade;
    int to;
    int from;
} saxa_move;

/*returns the position best move*/
saxa_move saxamove(Board board, int depth, int saxa_color);

/* gives value to a instant position*/

double evaluatePosition(Board board);

/* returns the best move of a position based on depth*/
saxa_move finalMoveGrade(Board board, int depth);

/* test values for position*/

double moveGrade(Board board, int from, int to, int depth);

/* this function returns a value between zero and one*/

static double sigmoid(double grade) {
    return 1.0 / (1.0 + pow(2.718281828, -grade));
}

///this is the call function for a SAXA play

saxa_move saxamove(Board board, int depth, int saxa_color) {
    saxaColor = saxa_color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = depth;
    return finalMoveGrade(board, depth);
}


saxa_move finalMoveGrade(Board board, int depth) {

    saxa_move move = { 0,-1,-1 };

    int movecounter = 0;

    for (int moveFrom = 0; moveFrom < 64; moveFrom++) {
        for (int moveTo = 0; moveTo < 64; moveTo++) {

            /*testing all the possible moves and storing the bigger value in a
            function,
            all these values are between zero and one, thanks to sigmoid,
            that means that we have an infinite value for checkmate or
            negative infinite value for an oponnent checkmate, after all,
            what is worse than losing a chess game?*/

            if (board.move.list[moveFrom][moveTo] == true) {
                movecounter++;
                double grade = moveGrade(board, moveFrom, moveTo, depth);

                if (board.state.whoMoves == saxaColor) {
                    if (grade > move.grade || movecounter == 1) {
                        move.grade = grade;
                        move.from = moveFrom;
                        move.to = moveTo;
                    }
                }
                else {
                    if (grade < move.grade || movecounter == 1) {
                        move.grade = grade;
                        move.from = moveFrom;
                        move.to = moveTo;
                    }
                }
            }
        }
    }

    return move;
}

double moveGrade(Board board, int from, int to, int depth) {

    /*right here is when the kid cry and his mom can`t see,
    makes the move and start the search,
    if he finds a mate stop the search,
    call a function that call this function while the depth
    is not 0,
    returns the value of the last position from the tree*/


    BoardMakeMove(&board, from, to, true);

    if (BoardKingInMate(board, saxaOpositeColor)) {
        return BEST_THING_POSSIBLE;
    }
    else if (BoardKingInMate(board, saxaColor)) {
        return WORST_THING_POSSIBLE;
    }


    if (depth > 0) {
        return finalMoveGrade(board, depth - 1).grade;
    }
    else
        return evaluatePosition(board);

}


double evaluatePosition(Board board) {

    /*this evaluate position is simple but effective,
    is impressive what SAXA can do just counting material,
    returns the position weight between 0 (any activity)
    or 1 (active)*/


    double grade = 0;
    int sinal;

    //contando material
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            int pieceType = PieceGetType(board.squares[rank * 8 + file]);
            int pieceColor = PieceGetColor(board.squares[rank * 8 + file]);
            sinal = (pieceColor == saxaColor) ? 1 : -1;

            switch (pieceType) {

            case PIECE_BISHOP:
                grade += bishopValue * sinal;
                break;
            case PIECE_KNIGHT:
                grade += horseValue * sinal;
                break;
            case PIECE_PAWN:
                grade += pawnValue * sinal;
                break;
            case PIECE_QUEEN:
                grade += queenValue * sinal;
                break;
            case PIECE_ROOK:
                grade += rookValue * sinal;
            default:
                break;

            }
        }
    }

    //counting squares of enemies attacks

    for (int i = 0; i < 64; i++) {
        if (board.move.pseudoLegalMoves[i] == true) {
            if (board.state.whoMoves != saxaColor) {
                grade += squareValue;
            }
            else if (board.state.whoMoves == saxaColor) {
                grade -= squareValue;
            }
        }
    }

    return sigmoid(grade);
}





#endif // SAXA_HPP_INCLUDED
