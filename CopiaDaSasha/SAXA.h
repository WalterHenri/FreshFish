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
    saxa_move move;

    bool finished;
};

struct ThreadMoveData threadMoveData;
struct ThreadMoveData threadMoveDataTest;

bool saxaThinking = false;
bool saxaThinkingTest = false;

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

bool calculationAbort = false;
bool testCalculationAbort = false;

saxa_move backtrackingMove(ChessBoard board, int depth, int saxa_color) {
    //saxaDepth = depth;
    return positionBestMove(board, depth, 0, 1);
}

HANDLE saxaMoveThreadId;
HANDLE saxaMoveTestThreadId;


DWORD WINAPI backtrackingMoveThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    //saxaDepth = moveData->depth;

    

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    moveData->move = positionBestMove(moveData->board, moveData->depth, -1, 2);
    end = clock();
    printf("Pruning Sorting Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the CPU time used
    printf("CPU time used: %f seconds\n", cpu_time_used);

  
    moveData->finished = true;
    return 0;
}

DWORD WINAPI backtrackingMoveTestThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    //saxaDepth = moveData->depth;



    //clock_t start, end;
    //double cpu_time_used;

    //start = clock();
    moveData->move = positionBestMoveTest(moveData->board, moveData->depth, -1, 2);
    //end = clock();
    //printf("Pruning Sorting Move(%d, %d) %f \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    //cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the CPU time used
    //printf("CPU time used: %f seconds\n", cpu_time_used);


    moveData->finished = true;
    return 0;
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





void swapMoves(saxa_move* a, saxa_move* b) {
    saxa_move temp = *a;
    *a = *b;
    *b = temp;
}

void quickSortMoves(saxa_move arr[], int low, int high) {
    if (low < high) {
        int pi = partitionMoves(arr, low, high);
        quickSortMoves(arr, low, pi - 1);
        quickSortMoves(arr, pi + 1, high);
    }
}

int partitionMoves(saxa_move arr[], int low, int high) {
    int pivot = arr[high].grade;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].grade >= pivot) {
            i++;
            swapMoves(&arr[i], &arr[j]);
        }
    }
    swapMoves(&arr[i + 1], &arr[high]);
    return (i + 1);
}


saxa_move positionBestMove(ChessBoard board, int depth, float alpha, float beta) {

    saxa_move move = { 0,-1,-1, 0};

    int moveCounter = 0;

    if (calculationAbort) return move;

    


    // Fetchin all moves
    // Aqui eu coloco todos os movimentos possiveis na posição em um array
    int size = board.move.count + board.move.promotionExtraCount;
    saxa_move* movesOrder = (saxa_move*) malloc(size * sizeof(saxa_move));

    if (size == 0) {
        printf("N tem pra onde ir\n");
    }

    for (int moveFrom = 0; moveFrom < 64; moveFrom++) {
        if (PieceHasType(board.squares[moveFrom], PIECE_NONE)) continue;
        if (!PieceHasColor(board.squares[moveFrom], board.state.whoMoves))  continue;
        for (int moveTo = 0; moveTo < 64; moveTo++) {

            if (board.move.list[moveFrom][moveTo] != MOVE_NONE) {

                int i = 5;
                if (board.move.list[moveFrom][moveTo] == MOVE_PAWN_PROMOTE) i = 2;

                for (; i <= 5; i++) {
                    saxa_move tryMove = { 0, moveFrom, moveTo, i};
                    tryMove.grade = moveGrade(board, tryMove, 0, alpha, beta);
                    movesOrder[moveCounter] = tryMove;
                    moveCounter++;
                }
            }
            

        }
    }
        
    //if (moveCounter > 0) {
        //movesOrder = (saxa_move*)realloc(movesOrder, sizeof(saxa_move) * moveCounter);
    //}


    // Se whoMoves == PIECE_WHITE vai tentar maximizar a nota
    // Se whoMoves == PIECE_BLACK vai tentar minimizar a nota


    // Ordering moves based on grade
    // Ordem decrescente, maiores notas primeiro
    
    /*
    for (int i = 0; i < moveCounter; i++) {
        for (int j = 1; j < moveCounter - i; j++) {

            if (movesOrder[j - 1].grade < movesOrder[j].grade) {
                saxa_move temp = movesOrder[j - 1];
                movesOrder[j - 1] = movesOrder[j];
                movesOrder[j] = temp;

            }
        }
    }
    */
    
    // Ordem decrescente
    quickSortMoves(movesOrder, 0, moveCounter - 1);

       

        
    // Aqui eu to dando uma nota inicial pro melhor movimento
    // Essa nota inicial tem que ser a piorzinha de todas
    // Pra garantir que o primeiro movimento seja considerado

    // Nota pessima se for PIECE_WHITE
    // Nota otima   se for PIECE_BLACK
    move.grade = 2;
    if (board.state.whoMoves == PIECE_WHITE) {
        move.grade = -1;
    }
        
      
    // Checa todos os movimentos na ordem de melhor para pior
    saxa_move tryMove;
    for (int i = 0; i < moveCounter ; i++) {

        // Se for PIECE_WHITE ler movimentos em ordem decrescente
        // Se for PIECE_BLACK ler movimentos em ordem crescente
        if (board.state.whoMoves == PIECE_WHITE) {
            tryMove = movesOrder[i];
        }
        else {
            tryMove = movesOrder[moveCounter - 1 - i];
        }

        tryMove.grade = moveGrade(board, tryMove, depth, alpha, beta);

        // Se for PIECE_WHITE substituir movimento atual por novo melhor movimento
        if (board.state.whoMoves == PIECE_WHITE) {
            if (tryMove.grade > move.grade) {
                move = tryMove;
            }

            alpha = max(alpha, tryMove.grade);
            if (beta <= alpha) {
                break;
            }
        }
        // Se for PIECE_BLACK substituir movimento atual por novo pior movimento
        else { 
            if (tryMove.grade < move.grade) {
                move = tryMove;
            }

            beta = min(beta, tryMove.grade);
            if (beta <= alpha) {
                break;
            }
        }
    }
        
    free(movesOrder);


    return move;
}


/// Returns the evaluation grade of the move in the position
double moveGrade(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta) {

    /*right here is when the kid cry and his mom can`t see,
    makes the move and start the search,
    if he finds a mate stop the search,
    call a function that call this function while the depth
    is not 0, returns the value of the last position from the tree*/ 


    BoardMakeMove(&board, tryMove.from, tryMove.to, tryMove.extra, true);


    if (BoardKingInMate(&board, PIECE_BLACK)) {
        return BEST_THING_POSSIBLE;
    }
    else if (BoardKingInMate(&board, PIECE_WHITE)) {
        return WORST_THING_POSSIBLE;
    }
    else if (boardInDraw(&board)) {
        return ONE_OF_THE_THINGS_POSSIBLE;
    }


    if (depth > 0) {
        return positionBestMove(board, depth - 1, alpha, beta).grade;
    }
    else
        return evaluatePosition(&board);

}







saxa_move positionBestMoveTest(ChessBoard board, int depth, float alpha, float beta) {

    saxa_move move = { 0,-1,-1, 0 };

    int moveCounter = 0;

    if (testCalculationAbort) return move;




    // Fetchin all moves
    // Aqui eu coloco todos os movimentos possiveis na posição em um array
    int size = board.move.count + board.move.promotionExtraCount;
    saxa_move* movesOrder = (saxa_move*)malloc(size * sizeof(saxa_move));

    if (size == 0) {
        printf("N tem pra onde ir\n");
    }

    for (int moveFrom = 0; moveFrom < 64; moveFrom++) {
        if (PieceHasType(board.squares[moveFrom], PIECE_NONE)) continue;
        if (!PieceHasColor(board.squares[moveFrom], board.state.whoMoves))  continue;
        for (int moveTo = 0; moveTo < 64; moveTo++) {

            if (board.move.list[moveFrom][moveTo] == MOVE_PAWN_PROMOTE) {
                for (int i = 2; i <= 5; i++) {
                    saxa_move tryMove = { 0, moveFrom, moveTo, i };
                    tryMove.grade = moveGradeTest(board, tryMove, 0, alpha, beta);
                    movesOrder[moveCounter] = tryMove;
                    moveCounter++;
                }
            }
            else if (board.move.list[moveFrom][moveTo] == true) {
                saxa_move tryMove = { 0, moveFrom, moveTo, 0 };
                tryMove.grade = moveGradeTest(board, tryMove, 0, alpha, beta);
                movesOrder[moveCounter] = tryMove;
                moveCounter++;
            }

        }
    }

    if (moveCounter > 0) {
        movesOrder = (saxa_move*)realloc(movesOrder, sizeof(saxa_move) * moveCounter);
    }


    // Se whoMoves == PIECE_WHITE vai tentar maximizar a nota
    // Se whoMoves == PIECE_BLACK vai tentar minimizar a nota


    // Ordering moves based on grade
    // if (board.state.whoMoves == PIECE_WHITE) {
    // Ordem decrescente, maiores notas primeiro
    for (int i = 0; i < moveCounter; i++) {
        for (int j = 1; j < moveCounter - i; j++) {

            if (movesOrder[j - 1].grade < movesOrder[j].grade) {
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

    // Nota pessima se for PIECE_WHITE
    // Nota otima   se for PIECE_BLACK
    move.grade = 2;
    if (board.state.whoMoves == PIECE_WHITE) {
        move.grade = -1;
    }


    // Checa todos os movimentos na ordem de melhor para pior
    saxa_move tryMove;
    for (int i = 0; i < moveCounter; i++) {

        // Se for PIECE_WHITE ler movimentos em ordem decrescente
        // Se for PIECE_BLACK ler movimentos em ordem crescente
        if (board.state.whoMoves == PIECE_WHITE) {
            tryMove = movesOrder[i];
        }
        else {
            tryMove = movesOrder[moveCounter - 1 - i];
        }

        tryMove.grade = moveGradeTest(board, tryMove, depth, alpha, beta);

        // Se for PIECE_WHITE substituir movimento atual por novo melhor movimento
        if (board.state.whoMoves == PIECE_WHITE) {
            if (tryMove.grade > move.grade) {
                move = tryMove;
            }

            alpha = max(alpha, tryMove.grade);
            if (beta <= alpha) {
                break;
            }
        }
        // Se for PIECE_BLACK substituir movimento atual por novo pior movimento
        else {
            if (tryMove.grade < move.grade) {
                move = tryMove;
            }

            beta = min(beta, tryMove.grade);
            if (beta <= alpha) {
                break;
            }
        }
    }

    free(movesOrder);


    return move;
}


/// Returns the evaluation grade of the move in the position
double moveGradeTest(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta) {

    /*right here is when the kid cry and his mom can`t see,
    makes the move and start the search,
    if he finds a mate stop the search,
    call a function that call this function while the depth
    is not 0, returns the value of the last position from the tree*/


    BoardMakeMove(&board, tryMove.from, tryMove.to, tryMove.extra, true);


    if (BoardKingInMate(&board, PIECE_BLACK)) {
        return BEST_THING_POSSIBLE;
    }
    else if (BoardKingInMate(&board, PIECE_WHITE)) {
        return WORST_THING_POSSIBLE;
    }
    else if (boardInDraw(&board)) {
        return ONE_OF_THE_THINGS_POSSIBLE;
    }


    if (depth > 0) {
        return positionBestMoveTest(board, depth - 1, alpha, beta).grade;
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

    return sigmoid(grade);
}

