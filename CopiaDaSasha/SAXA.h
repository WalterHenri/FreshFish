#include "FreshFish.h"
#include "Evaluate.h"

/**            SAXA

 Sistema Artificial de Xadrez Academico.
 The greatest chess engine of all time,

 XUXA built in,

 **/

 /* a bot that knows how to play chess.*/

///this is the call function for a SAXA play

saxa_move backtrackingMove(ChessBoard board, int depth, int saxa_color) {
    //saxaDepth = depth;
    return positionBestMove(board, depth, INT_MIN, INT_MAX);
}

HANDLE saxaMoveThreadId;
//HANDLE saxaMoveTestThreadId;

bool memoActive = false;

DWORD WINAPI backtrackingMoveThreaded(void* data) {

    struct ThreadMoveData* moveData = (struct ThreadMoveData*)data;

    clock_t start, end;
    double cpu_time_used;

    printf("*------------Calculation Start------------*\n");
    
    memoActive = true;
    start = clock();
    moveData->move = positionBestMove(moveData->board, moveData->depth, INT_MIN, INT_MAX);
    end = clock();


    if (moveData->move.grade >= BEST_THING_POSSIBLE) {
        int mateIn = moveData->depth - (moveData->move.grade - BEST_THING_POSSIBLE);
        printf("BestMove (%d, %d) [M%d] PruningSortingMemorizing \n", moveData->move.from, moveData->move.to, mateIn);
    }
    else if (moveData->move.grade <= WORST_THING_POSSIBLE) {
        int mateIn = moveData->depth - (WORST_THING_POSSIBLE - moveData->move.grade);
        printf("BestMove (%d, %d) [-M%d] PruningSortingMemorizing \n", moveData->move.from, moveData->move.to, mateIn);
    }
    else {
        printf("BestMove (%d, %d) [%d] PruningSortingMemorizing \n", moveData->move.from, moveData->move.to, moveData->move.grade);
    }
        

    
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the CPU time used
    printf("CPU time used: %f seconds\n", cpu_time_used);
    printf("\n");

  
    moveData->finished = true;
    return 0;
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




saxa_move positionBestMove(ChessBoard board, int depth, int alpha, int beta) {

    saxa_move move = { 0,-1,-1, 0};

    int moveCounter = 0;

    if (calculationAbort) return move;

    
    if (memoActive) {
        struct MemoEvaluation* m = search(boardToKey(&board));
        if (m != NULL) {
            //printf("Economizando! Nota: %d\n", m->grade);
            return m->bestMove;
        }
    }
    



    // Fetchin all moves
    // Aqui eu coloco todos os movimentos possiveis na posi��o em um array
    int size = board.move.count + board.move.promotionExtraCount;
    saxa_move* movesOrder = (saxa_move*) malloc(size * sizeof(saxa_move));

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
     



    // Ordem decrescente
    quickSortMoves(movesOrder, 0, moveCounter - 1);

    // Aqui eu to dando uma nota inicial pro melhor movimento
    // Essa nota inicial tem que ser a piorzinha de todas
    // Pra garantir que o primeiro movimento seja considerado

    // Nota pessima se for PIECE_WHITE
    // Nota otima   se for PIECE_BLACK
    move.grade = INT_MAX;
    if (board.state.whoMoves == PIECE_WHITE) {
        move.grade = INT_MIN;
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

    if (memoActive) {
        struct MemoEvaluation* m = search(boardToKey(&board));
        if (m != NULL) {
            insert(boardToKey(&board), move.grade, move);
        }
    }
    
   
    return move;
}


/// Returns the evaluation grade of the move in the position
int moveGrade(ChessBoard board, saxa_move tryMove, int depth, int alpha, int beta) {

    /*right here is when the kid cry and his mom can`t see,
    makes the move and start the search,
    if he finds a mate stop the search,
    call a function that call this function while the depth
    is not 0, returns the value of the last position from the tree*/ 


    BoardMakeMove(&board, tryMove.from, tryMove.to, tryMove.extra, true);


    if (BoardKingInMate(&board, PIECE_BLACK)) {
        return BEST_THING_POSSIBLE + min(depth, 98);
    }
    else if (BoardKingInMate(&board, PIECE_WHITE)) {
        return WORST_THING_POSSIBLE - min(depth, 98);
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












