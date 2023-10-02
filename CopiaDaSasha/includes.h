#pragma once

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
saxa_move finalMoveGrade(Board board, int depth, float alpha, float beta);

/* test values for position*/

double moveGrade(Board board, int from, int to, int depth, float alpha, float beta);

/* this function returns a value between zero and one*/

static double sigmoid(double grade) {
    return 1.0 / (1.0 + pow(2.718281828, -grade));
}
