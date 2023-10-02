#pragma once
#include "includes.h"

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
bool pruning = false;
bool moveSorting = false;


DWORD WINAPI saxaMoveThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    saxaColor = moveData->color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = moveData->depth;

    /*

    pruning = true;
    moveSorting = true;
    moveData->move = finalMoveGrade(moveData->board, moveData->depth, 0, 1);
    printf("Pruning Sorting Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);*/

    moveSorting = false;
    pruning = true;
    moveData->move = finalMoveGrade(moveData->board, moveData->depth, 0, 1);
    printf("Pruning Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);

    /*
    moveSorting = false;
    pruning = false;
    moveData->move = finalMoveGrade(moveData->board, moveData->depth, 0, 1);
    printf("Normal Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    */

    
    moveData->finished = true;



    return NULL;
}




saxa_move finalMoveGrade(Board board, int depth, float alpha, float beta) {

    saxa_move move = { 0,-1,-1 };

    int moveCounter = 0;

    
    if (moveSorting) {

        saxa_move* movesOrder;

        movesOrder = (saxa_move*) malloc(sizeof(saxa_move) * 64 * 64);
        if (movesOrder == NULL) {
            printf("F é nulo\n");
        }

        printf("Rodou a linha %d\n",__LINE__);

        for (int moveFrom = 0; moveFrom < 64; moveFrom++) {

            for (int moveTo = 0; moveTo < 64; moveTo++) {
                if (board.move.list[moveFrom][moveTo] == true) {
                    

                    double grade = 0;

                    grade = moveGrade(board, moveFrom, moveTo, 0, alpha, beta);
                    saxa_move xMove;
                    xMove.from = moveFrom;
                    xMove.to = moveTo;
                    xMove.grade = grade;

                    printf("MoveCounter = %d :%d\n", moveCounter, __LINE__);
                    movesOrder[moveCounter] = xMove;
                    printf("Botou dentro do movesOrder :%d\n", __LINE__);
                    moveCounter++;
                }
            }
        }

        printf("Rodou a linha antes do REalloc %d\n", __LINE__);
        movesOrder = (saxa_move*)realloc(movesOrder, sizeof(saxa_move) * moveCounter);
        printf("Rodou a linha do REalloc %d\n", __LINE__);

        // Ordering
        /*
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
        */


        for (int i = 0; i < moveCounter; i++) {

            int moveFrom = movesOrder[i].from;
            int moveTo = movesOrder[i].to;

            double grade = moveGrade(board, moveFrom, moveTo, depth, alpha, beta);

            if (board.state.whoMoves == saxaColor) {
                if (grade > move.grade || i == 0) {
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
                if (grade < move.grade || i == 0) {
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

        free(movesOrder);

        return move;

    }
    else {

        for (int moveFrom = 0; moveFrom < 64; moveFrom++) {

            if (PieceHasType(board.squares[moveFrom], PIECE_NONE)) continue;
            if (!PieceHasColor(board.squares[moveFrom], board.state.whoMoves))  continue;

            for (int moveTo = 0; moveTo < 64; moveTo++) {

                /*testing all the possible moves and storing the bigger value in a
                function,
                all these values are between zero and one, thanks to sigmoid,
                that means that we have an infinite value for checkmate or
                negative infinite value for an oponnent checkmate, after all,
                what is worse than losing a chess game?*/

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

    }

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

