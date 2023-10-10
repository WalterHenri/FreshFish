#pragma once
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "Board.h"
#include "Piece.h"

#define IMESUARABLE 1
#define WORST_THING_POSSIBLE 0
#define ONE_OF_THE_THINGS_POSSIBLE 0.5;
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
int saxaColor = PIECE_BLACK;

typedef struct {
    double grade;
    int from;
    int to;
    int extra;
} saxa_move;

/*returns the position best move*/
saxa_move backtrackingMove(ChessBoard board, int depth, int saxa_color);

/* gives value to an instant position*/
double evaluatePosition(ChessBoard* board);

/* returns the best move of a position based on depth*/
saxa_move positionBestMove(ChessBoard board, int depth, float alpha, float beta);
saxa_move positionBestMoveTest(ChessBoard board, int depth, float alpha, float beta);

/* test values for position*/
double moveGrade(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta);
double moveGradeTest(ChessBoard board, saxa_move tryMove, int depth, float alpha, float beta);

/* this function returns a value between zero and one*/
static double sigmoid(double grade) {
    return 1.0 / (1.0 + pow(2.718281828, -grade));
}

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <windows.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif



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