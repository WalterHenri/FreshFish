#pragma once
#include "includes.h"

/**            SAXA

 Sistema Artificial de Xadrez Academico.
 The greatest chess engine of all time,

 XUXA built in,

 **/

 /* a bot that knows how to play chess.*/




struct ThreadMoveData {
    ChessBoard board;
    int depth;
    int color;
    saxa_move move;

    bool finished;
};

struct ThreadMoveData threadMoveData;

bool saxaThinking = false;

// Biblioteca do windows pra fazer thread
#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <windows.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

///this is the call function for a SAXA play

saxa_move backtrackingMove(ChessBoard board, int depth, int saxa_color) {
    saxaColor = saxa_color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = depth;
    return positionBestMove(board, depth, 0, 1);
}

HANDLE saxaMoveThreadId;


DWORD WINAPI backtrackingMoveThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    saxaColor = moveData->color;
    saxaOpositeColor = (saxaColor == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    saxaDepth = moveData->depth;

    

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    moveData->move = positionBestMove(moveData->board, moveData->depth, -1, 2);
    end = clock();
    printf("Pruning Sorting Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the CPU time used
    printf("CPU time used: %f seconds\n", cpu_time_used);

  
    moveData->finished = true;
    return NULL;
}

char* nextMoveOpenings(char* moves,int size) {
    
    


}

void fenToBoardMove(char* move, Board* board) {

}

char* boardMoveToFen(ChessBoard board, int from, int to) {
   
    char pieces[] = {'K','Q','B','N','R'};
    char files[] = { 'a','b','c','d','e','f','g','h' };


    char* string = (char*)malloc(sizeof(char)*9);
    switch (PieceGetType(board.squares[from])) {
    case PIECE_PAWN:
        string[0] = files[PieceFile(from)];
        if (PieceGetType(board.squares[to]) == PIECE_NONE)
            string[1] = PieceRank(to);
        else {
            string[1] = 'x';
            if(PieceGetType(board.squares[to]) == PIECE_PAWN){
                string[2] = files[PieceFile(to)];
                string[3] = PieceRank(to) + '0';
            }
            else {

            }

        }

        
    }
    
}



saxa_move positionBestMove(ChessBoard board, int depth, float alpha, float beta) {

    saxa_move move = { 0,-1,-1 };

        int moveCounter = 0;

        // Fetchin all moves
        

        // Aqui eu coloco todos os movimentos possiveis na posição em um array
        int size = board.move.count;
        saxa_move* movesOrder = (saxa_move*) malloc(size * sizeof(saxa_move));

        if (size == 0) {
            printf("N tem pra onde ir\n");
        }

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
        
        if (moveCounter > 0) {
            movesOrder = (saxa_move*)realloc(movesOrder, sizeof(saxa_move) * moveCounter);
        }


        // Ordering moves based on grade
       // if (board.state.whoMoves == PIECE_WHITE) {
            for (int i = 0; i < moveCounter; i++) {
                for (int j = 1; j < moveCounter - i; j++) {

                    if (movesOrder[j - 1].grade > movesOrder[j].grade) {
                        saxa_move temp = movesOrder[j - 1];
                        movesOrder[j - 1] = movesOrder[j];
                        movesOrder[j] = temp;

                    }
                }
            }
       // }
        /*
        else{
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
       

        
        // Aqui eu to dando uma nota inicial pro melhor movimento
        // Essa nota inicial tem que ser a piorzinha de todas
        // Pra garantir que o primeiro movimento seja considerado
        move.grade = 2;
        if (board.state.whoMoves == saxaColor) {
            move.grade = -1;
        }
        
      
        // Checa todos os movimentos na ordem de melhor para pior
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


    return move;
}


/// Returns the evaluation grade of the move in the position
double moveGrade(ChessBoard board, int from, int to, int depth, float alpha, float beta) {

    /*right here is when the kid cry and his mom can`t see,
    makes the move and start the search,
    if he finds a mate stop the search,
    call a function that call this function while the depth
    is not 0,
    returns the value of the last position from the tree*/ 


    BoardMakeMove(&board, from, to, true);


    if (BoardKingInMate(&board, saxaOpositeColor)) {
        return BEST_THING_POSSIBLE;
    }
    else if (BoardKingInMate(&board, saxaColor)) {
        return WORST_THING_POSSIBLE;
    }
    else if (boardInDraw(&board)) {
        return ONE_OF_THE_THINGS_POSSIBLE;
    }
    else if (board.state.waitPromotion) {
        board.state.waitPromotion = 0;
        double bestGrade = 0;
        int index = 2;
        for (int i = 2; i <= 5; i++) {
            board.squares[to] = PieceGetColor(board.squares[to]) + i;
            double g = positionBestMove(board, depth - 1, alpha, beta).grade;
            if (g > bestGrade) {
                bestGrade = g;
                index = i;
            }
        }
        board.squares[to] = index;
        return bestGrade;
    }


    if (depth > 0) {
        return positionBestMove(board, depth - 1, alpha, beta).grade;
    }
    else
        return evaluatePosition(&board);

}


double evaluatePosition(ChessBoard* board) {

    /*this evaluate position is simple but effective,
    is impressive what SAXA can do just counting material,
    returns the position weight between 0 (any activity)
    or 1 (active)*/


    double grade = 0;
    int sinal;

    //contando material
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            int pieceType = PieceGetType(board->squares[rank * 8 + file]);
            int pieceColor = PieceGetColor(board->squares[rank * 8 + file]);
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

    // Counting squares of enemies attacks
    for (int i = 0; i < 64; i++) {
        if (board->move.pseudoLegalMoves[i] == true) {
            if (board->state.whoMoves != saxaColor) {
                grade += squareValue;
            }
            else if (board->state.whoMoves == saxaColor) {
                grade -= squareValue;
            }
        }
    }

    return sigmoid(grade);
}

