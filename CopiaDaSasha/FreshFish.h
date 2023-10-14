#pragma once
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "Board.h"
#include "Piece.h"

int saxaDepth; /// warning!!!
int saxaColor = PIECE_WHITE;

typedef struct {
    //double grade;
    int grade;
    int from;
    int to;
    int extra;
} saxa_move;

/*returns the position best move*/
saxa_move backtrackingMove(ChessBoard board, int depth, int saxa_color);

/* gives value to an instant position*/
//double evaluatePosition(ChessBoard* board);
int evaluatePosition(ChessBoard* board);

/* returns the best move of a position based on depth*/
//saxa_move positionBestMove(ChessBoard board, int depth, float alpha, float beta);
//saxa_move positionBestMoveTest(ChessBoard board, int depth, float alpha, float beta);
saxa_move positionBestMove(ChessBoard board, int depth, int alpha, int beta);
saxa_move positionBestMoveTest(ChessBoard board, int depth, int alpha, int beta);

/* test values for position*/
//double moveGrade(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta);
//double moveGradeTest(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta);
int moveGrade(ChessBoard board, saxa_move tryMove, int depth, int alpha, int beta);
int moveGradeTest(ChessBoard board, saxa_move tryMove, int depth, int alpha, int beta);

//#define PlaySound RaylibPlaySound

#define WIN32_LEAN_AND_MEAN
#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <windows.h> // or any library that uses Windows.h




#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

//#define PlaySound PlaySoundWindows

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

bool calculationAbort = false;
bool testCalculationAbort = false;
