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
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

///this is the call function for a SAXA play

saxa_move saxamove(Board board, int depth, int saxa_color) {
    saxaColor = saxa_color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = depth;
    return finalMoveGrade(board, depth, 0, 1);
}

struct ThreadMoveData {
    Board board;
    int depth;
    int color;
    saxa_move move;

    bool finished;
};
struct ThreadMoveData threadMoveData;

bool saxaThinking = false;


#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <windows.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif



HANDLE saxaMoveThreadId;


DWORD WINAPI saxaMoveThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    saxaColor = moveData->color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = moveData->depth;

    

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    moveData->move = finalMoveGrade(moveData->board, moveData->depth, 0, 1);
    end = clock();
    printf("Pruning Sorting Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the CPU time used
    printf("CPU time used: %f seconds\n", cpu_time_used);

  
    moveData->finished = true;
    return NULL;
}




saxa_move finalMoveGrade(Board board, int depth, float alpha, float beta) {

    saxa_move move = { 0,-1,-1 };

    //if (moveSorting) {
        int moveCounter = 0;

        // Fetchin all moves
        int size = (64*64) * sizeof(saxa_move);
        saxa_move* movesOrder = (saxa_move*) malloc(size);

        for (int moveFrom = 0; moveFrom < 64; moveFrom++) {
            if (PieceHasType(board.squares[moveFrom], PIECE_NONE)) continue;
            if (!PieceHasColor(board.squares[moveFrom], board.state.whoMoves))  continue;
            for (int moveTo = 0; moveTo < 64; moveTo++) {
                if (board.move.list[moveFrom][moveTo] == true) {
                    double grade = 0;

                    grade = moveGrade(board, moveFrom, moveTo, 0, alpha, beta);
                    movesOrder[moveCounter].from = moveFrom;
                    movesOrder[moveCounter].to = moveTo;
                    movesOrder[moveCounter].grade = grade;
                    moveCounter++;
                }
            }
        }
        if (moveCounter != 0) {
            movesOrder = (saxa_move*)realloc(movesOrder, sizeof(saxa_move) * moveCounter);
        }
        
        // Ordering moves based on grade
        if (board.state.whoMoves == PIECE_WHITE) {
            for (int i = 0; i < moveCounter; i++) {
                for (int j = 1; j < moveCounter - i; j++) {

                    if (movesOrder[j - 1].grade > movesOrder[j].grade) {
                        saxa_move temp = movesOrder[j - 1];
                        movesOrder[j - 1] = movesOrder[j];
                        movesOrder[j] = temp;

                    }
                }
            }
        }
        else {
            for (int i = 0; i < moveCounter; i++) {
                for (int j = 1; j < moveCounter - i; j++) {

                    if (movesOrder[j - 1].grade < movesOrder[j].grade) {
                        saxa_move temp = movesOrder[j - 1];
                        movesOrder[j - 1] = movesOrder[j];
                        movesOrder[j] = temp;

                    }
                }
            }
        }

        
        if (board.state.whoMoves == saxaColor) {
            move.grade = 0;
        }
        else {
            move.grade = 1;
        }
        
      
        for (int i = 0; i < moveCounter ; i++) {
            int moveFrom = movesOrder[i].from;
            int moveTo = movesOrder[i].to;

            if (!board.state.whoMoves == PIECE_WHITE) {
                moveFrom = movesOrder[moveCounter-1-i].from;
                moveTo = movesOrder[moveCounter - 1 - i].to;
            }

            double grade = moveGrade(board, moveFrom, moveTo, depth, alpha, beta);

            if (board.state.whoMoves == saxaColor) {
                if (grade > move.grade) {
                    move.grade = grade;
                    move.from = moveFrom;
                    move.to = moveTo;
                }


                    alpha = max(alpha, grade);
                    if (beta <= alpha) {
                        break;
                    }
                

            }
            else { 
                if (grade < move.grade) {
                    move.grade = grade;
                    move.from = moveFrom;
                    move.to = moveTo;
                }


                    beta = min(beta, grade);
                    if (beta <= alpha) {
                        break;
                    }
                
            }
            
            
        }
        
        free(movesOrder);
        
    //}
    //else {

        /*
        for (int moveFrom = 0; moveFrom < 64; moveFrom++) {

            if (PieceHasType(board.squares[moveFrom], PIECE_NONE)) continue;
            if (!PieceHasColor(board.squares[moveFrom], board.state.whoMoves))  continue;

            for (int moveTo = 0; moveTo < 64; moveTo++) {

                //testing all the possible moves and storing the bigger value in a
                //function,
                //all these values are between zero and one, thanks to sigmoid,
                //that means that we have an infinite value for checkmate or
                //negative infinite value for an oponnent checkmate, after all,
                //what is worse than losing a chess game?

                if (board.move.list[moveFrom][moveTo] == true) {
                    moveCounter++;
                    double grade = moveGrade(board, moveFrom, moveTo, depth, alpha, beta);

                    if (board.state.whoMoves == saxaColor) {
                        if (grade > move.grade || moveCounter == 1) {
                            move.grade = grade;
                            move.from = moveFrom;
                            move.to = moveTo;
                        }

                        if (pruning) {
                            alpha = max(alpha, grade);
                            if (beta <= alpha) {
                                moveFrom = 64;
                                break;
                            }
                        }

                    }
                    else {
                        if (grade < move.grade || moveCounter == 1) {
                            move.grade = grade;
                            move.from = moveFrom;
                            move.to = moveTo;
                        }

                        if (pruning) {
                            beta = min(beta, grade);
                            if (beta <= alpha) {
                                moveFrom = 64;
                                break;
                            }
                        }
                    }
                }
            }
        }
        */
    //}

    return move;
}

double moveGrade(Board board, int from, int to, int depth, float alpha, float beta) {

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
        return finalMoveGrade(board, depth - 1, alpha, beta).grade;
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

