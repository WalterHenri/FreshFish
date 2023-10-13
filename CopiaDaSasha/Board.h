#pragma once
#ifndef BOARD_HPP
#define BOARD_HPP


#include <stdio.h>
#include <stdbool.h>

#include "raylib.h"


#define BOARD_SQUARE_LENGTH_FACTOR 10.f

#define BOARD_FEN_LENGTH 85
#define BOARD_FEN_DEFAULT "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/* Define this disable any graphics output and enable Performance Test, move
 * enumeration (Perft) mode.
 */
 /* #define BOARD_PERFT */

typedef enum {
    MOVE_NONE,
    MOVE_NORMAL,

    MOVE_PAWN_TWO_FORWARD,
    MOVE_PAWN_EN_PASSANT,
    MOVE_PAWN_PROMOTE,

    MOVE_CASTLING_KING,
    MOVE_CASTLING_QUEEN,

} MoveType;

typedef enum {
    CASTLING_NONE,
    CASTLING_KING_SIDE,
    CASTLING_QUEEN_SIDE,
} CastlingType;

typedef struct {

    bool isLogged;
    char name[50];
    int rating;
    char email[50];
    int id;

} Profile;

typedef struct {

    int squares[64];

    struct {
        int enPassantSquare;
        int castlingBlack;
        int castlingWhite;
        int halfmoves;
        int fullmoves;
        int whoMoves;
    } state;

    int kingSquare[2];

    struct {
        /* Number of generated moves */
        int count;

        int promotionExtraCount;

        /* List of squares that are attacked by the enemy */
        bool attackSquares[64];

        /* List of move that can be made for which square (the move for the type of
        * the piece in some square)
        */
        int list[64][64];
    } move;
} ChessBoard;

typedef struct {
    /* The position that's start draw the board */
    Vector2 drawPosition;

    bool backButtonClicked;

    /* The color of the squares */
    Color squareBlackColor;
    Color squareWhiteColor;

    /* Pieces sprite sheet and the size of a sprite */
    Texture2D pieceSpriteSheet;
    Vector2 pieceSpriteSize;

    /* The length in pixels of a single square and the square data, i.e., if has
     * piece or not and which type of piece
     */
    float squareLength;

    /* Drag information store if is dragging and which piece is dragging */
    struct {
        int position;
        bool dragging;
        bool selecting;
        float ringRotation;
    } movingPiece;

    struct {
        bool active;
        int from;
        int to;
    } promotion;

    /* Can be used to inform if the board was recently updated */
    bool updated;

    /* Board viewing perspective */
    bool viewAsWhite;

    /* Board rotating variables */
    bool xRotating;
    float xAngle;

    float yScale;
    float xScale;
    
    /* Evaluation bar variables */
    int positionGradeDepth;
    float positionGrade;

    /* Actual game board */
    ChessBoard chessBoard;

    /* Board that is shown on screen */
    ChessBoard displayChessBoard;

    /* Keeps track of the FEN of the previous positions */
    char prevFen[100][100];
    int  prevFenTotal;
    int  prevFenIndex;

} Board;

void login(int* menuorboard);

void GameBoard(int* menuorboard);

void gameDificult(int* menuorboard);

void menu(int* menuorboard);

void gamemode(int* menuorboard);

void menuInit();

// Inicializa um novo tabuleiro
Board BoardInit(int screenWidth, int screenHeight);

// Reinicializa um tabuleiro
void _BoardInit(Board* board);

// Carrega a FEN de uma string
bool BoardLoadFEN(ChessBoard* board, const char fen[BOARD_FEN_LENGTH]);

// Carrega a FEN de um arquivo
bool _BoardLoadFEN(ChessBoard* board);

void BoardGetAsFEN(ChessBoard* board, char fenString[100]);

char* boardMoveToFen(ChessBoard board, int from, int to);

void BoardUnload(Board* board);

void BoardResize(Board* board, int screenWidth, int screenHeight);

bool BoardMakeMove(ChessBoard* board, int from, int to, int extra, bool updateWhoMoves);

bool BoardKingInCheck(ChessBoard* board, int kingColor);

bool BoardKingInMate(ChessBoard* board, int kingColor);

int BoardPerft(Board* board, int depth);

void BoardUpdate(Board* board);

void BoardDraw(Board* board, int* menu);

#endif /* BOARD_HPP */

