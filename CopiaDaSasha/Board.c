#pragma warning(disable : 4996)
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "SAXA.h"
#include "raylib.h"
#include "board.h"
#include "Piece.h"

int counter;//variavel para contar os char digitados
int counterR;
int countere;

bool istexting;
bool istextingr;
bool istextinge;

int selectionB;

int saxaDephtBoard;

int quadros;
bool isSinglePlayer;

#include "Recursos.h"


#include "cardapio.h"

/* Internal helpers functions
 */
static void findKings(ChessBoard* board);
static void generateMoves(ChessBoard* board, bool onlyLegalMoves);
static void updateTurn(ChessBoard* board, bool updateWhoMove);
static bool isValidMove(ChessBoard board, int from, int to);

/* Internal generate move functions
 * This functions generate moves for specific pieces
 */
static void generateKingMoves(ChessBoard* board, int square, bool legal);
static void generateSlightMoves(ChessBoard* board, int square, bool legal);
static void generateKnightMoves(ChessBoard* board, int square, bool legal);
static void generatePawnMoves(ChessBoard* board, int square, bool legal);

/* Internal update functions
 */
static void updatePromotionMenu(Board* board);
static void backButton(Board* board, int* menu);

/* Internal draw functions
 */
static void drawSquare(Board board, Rectangle drawPosition, Color color);
static void drawPiece(Board board, Rectangle drawPosition, Color color, int piece, bool shrinked);
static void drawSelectRing(Board board, int centerX, int centerY, float size, Color color);
static void drawRankAndFiles(Board board, Rectangle drawPosition, int square, Color color);
static void drawMoves(Board board, ChessBoard chessBoard, Rectangle drawPosition, int square);
static void drawDraggingPiece(Board board, ChessBoard chessBoard, Rectangle drawPosition);
static void drawMateWindow(Board* board, int* menu);
static void drawPromotionMenu(Board board);



void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void myitoa(int n, char s[]) {
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}


void updateSetPosition(Board* board) {

    /* Resize the board if screen size has changed */
    if (IsWindowResized())
        BoardResize(board, GetScreenWidth(), GetScreenHeight());

    if (board->backButtonClicked) {
        // Essa parte está sendo implementada em backButton
    }
    else {

        if (GetMouseX() >= board->drawPosition.x && GetMouseX() < board->drawPosition.x + board->squareLength * 8 &&
            GetMouseY() >= board->drawPosition.y && GetMouseY() < board->drawPosition.y + board->squareLength * 8) {

            int rank = (GetMouseY() - board->drawPosition.y) / board->squareLength;
            int file = (GetMouseX() - board->drawPosition.x) / board->squareLength;
            if (!board->viewAsWhite) {
                rank = 7 - rank;
            }


            int square = PieceSquare(rank, file);
            static bool clicked = false;

            if (rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    board->chessBoard.squares[square] = board->editor.pieceType;
                }
                else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    board->chessBoard.squares[square] = PIECE_NONE;
                }
                char fenString[100];
                BoardGetAsFEN(&board->chessBoard, fenString);
                BoardLoadFEN(&board->displayChessBoard, fenString);
            }
        }
    }

    
    if (board->movingPiece.dragging || board->movingPiece.selecting)
        board->movingPiece.ringRotation += 150 * GetFrameTime();
    else
        board->movingPiece.ringRotation = 0;

    if (board->movingPiece.ringRotation > 360)
        board->movingPiece.ringRotation = 0;
    
}

void showFen(Board* board) {

}


void setupPosition(Board* board, int* menu) {

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();


    //Board* board = (Board *) malloc(sizeof(Board));
    
    //BoardInit(screenWidth, screenHeight);

    //setup Position Mode is hard
    updatePieceButtons(board);
    updateSetPosition(board);
    

    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(0, 0);

   
    BoardDraw(board, menu);
    drawPieceButtons(board);

    EndDrawing();

    //quanta coisa!
    //buttonSaveFen();
    //buttonStartPlaying();
    //buttonSettings();
    
}




Board BoardInit(int screenWidth, int screenHeight) {
    Board board;
    memset(&board, 0, sizeof(Board));

    board.squareBlackColor = DARKGRAY;
    board.squareWhiteColor = WHITE;

    board.pieceSpriteSheet = LoadTexture("assets/chess_pieces.png");
    board.pieceSpriteSize.x = board.pieceSpriteSheet.width / 6.f;
    board.pieceSpriteSize.y = board.pieceSpriteSheet.height / 2.f,

    board.yScale = 1;
    board.xScale = 1;

  

    board.chessBoard.state.fullmoves = 1;
    board.viewAsWhite = true;

    BoardResize(&board, screenWidth, screenHeight);
    _BoardLoadFEN(&board.chessBoard);
    
    // Carregando o tabuleiro de Display
    char fenString[100];
    BoardGetAsFEN(&board.chessBoard, fenString);
    for (int i = 0; i < 100; i++) {
        board.prevFen[0][i] = fenString[i];
    }

    board.prevFenTotal++;
    BoardLoadFEN(&board.displayChessBoard, fenString);


    return board;
}

void _BoardInit(Board* board) {
    
    // Board Reset
    board->backButtonClicked = false;
    board->viewAsWhite = true;
    memset(&board->promotion, 0, sizeof board->promotion);
    board->xRotating = false;
    board->xAngle = 0;
    board->xScale = 1;
    board->yScale = 1;
    memset(&board->movingPiece, 0, sizeof board->movingPiece);
    
    // Chess Board Reset
    memset(&board->chessBoard.move, 0, sizeof board->chessBoard.move);
    memset(&board->chessBoard.state, 0, sizeof board->chessBoard.state);
    BoardLoadFEN(&board->chessBoard, BOARD_FEN_DEFAULT);

    // Display Board Reset
    memset(&board->displayChessBoard, 0, sizeof board->displayChessBoard); 
    memset(&board->prevFen, 0, sizeof board->prevFen);
    board->prevFenTotal = 0;
    board->prevFenIndex = 0;
    char fenString[100];
    BoardGetAsFEN(&board->chessBoard, fenString);
    for (int i = 0; i < 100; i++) {
        board->prevFen[0][i] = fenString[i];
    }
    board->prevFenTotal++;
    BoardLoadFEN(&board->displayChessBoard, fenString);
}

char* boardMoveToFen(ChessBoard board, int from, int to) {
    char pieces[] = { 'K', 'Q', 'B', 'N', 'R' };
    char files[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

    char* string = (char*)malloc(sizeof(char) * 9);
    int index = 0;

    Piece fromPiece = board.squares[from];
    Piece toPiece = board.squares[to];

    if (PieceGetType(fromPiece) == PIECE_PAWN) {
        if (PieceFile(from) != PieceFile(to) && toPiece == PIECE_NONE) {
            string[index++] = files[PieceFile(from)]; // Add the source file if it's not a capture
        }
        if (toPiece != PIECE_NONE) {
            string[index++] = 'x';
        }
        string[index++] = files[PieceFile(to)];
        string[index++] = '1' + (7 - PieceRank(to)); // Add target rank

        // Handle pawn promotion (e.g., "e8=Q")
        if (PieceRank(to) == 0 || PieceRank(to) == 7) {
            string[index++] = '=';
            string[index++] = pieces[PieceGetType(toPiece) - 1];
        }
    }
    else {
        string[index++] = pieces[PieceGetType(fromPiece) - 1];
        if (toPiece != PIECE_NONE) {
            string[index++] = 'x';
        }
        string[index++] = files[PieceFile(to)];
        string[index++] = '1' + (7 - PieceRank(to)); // Add target rank
    }

    // Null-terminate the string
    string[index] = '\0';

    return string;
}


bool BoardLoadFEN(ChessBoard* board, const char fen[BOARD_FEN_LENGTH]) {
    const char pieces_data[12] = {
        'k', PIECE_KING,
        'q', PIECE_QUEEN,
        'b', PIECE_BISHOP,
        'n', PIECE_KNIGHT,
        'r', PIECE_ROOK,
        'p', PIECE_PAWN,
    };

    int i = 0;
    int rank = 0;
    int file = 0;

    memset(board->squares, 0, sizeof(board->squares));

    /* Load the pieces location */
    for (; fen[i] != ' '; i++) {
        if (fen[i] == '/') {
            rank++;
            file = 0;
        }
        else if (isdigit(fen[i])) {
            /* Convert to a number and add to file */
            file += fen[i] - '0';
        }
        else {
            for (int j = 0; j < 12; j += 2) {
                /* if the fen symbol isn't the same of pieces data the jump to
                 * continue to the next pieces data symbol
                 */
                if (tolower(fen[i]) != pieces_data[j])
                    continue;

                board->squares[PieceSquare(rank, file)] = pieces_data[j + 1] + (isupper(fen[i]) ? PIECE_WHITE : PIECE_BLACK);
                file++;

                break;
            }
        }
    }

    /* Load who should move */
    i++;
    if (fen[i] == 'w')
        board->state.whoMoves = PIECE_WHITE;
    else if (fen[i] == 'b')
        board->state.whoMoves = PIECE_BLACK;
    else
        return false;

    for (i += 2; fen[i] != ' '; i++) {
        if (fen[i] == 'K')
            board->state.castlingWhite |= CASTLING_KING_SIDE;
        else if (fen[i] == 'Q')
            board->state.castlingWhite |= CASTLING_QUEEN_SIDE;
        else if (fen[i] == 'k')
            board->state.castlingBlack |= CASTLING_KING_SIDE;
        else if (fen[i] == 'q')
            board->state.castlingBlack |= CASTLING_QUEEN_SIDE;
        else if (fen[i] == '-')
            break;
        else
            return false;
    }

    i++;
    if (fen[i] != '-') {
        board->state.enPassantSquare = fen[i] - 'a' + (8 - (fen[i + 1] - '0')) * 8;
        i += 2;
    }
    else {
        board->state.enPassantSquare = 0;
        i++;
    }



    board->state.halfmoves = 0;
    board->state.fullmoves = 0;

    i++;
    for (; fen[i] != ' '; i++) {

        board->state.halfmoves = board->state.halfmoves * 10 + (fen[i] - '0');
    }

    i++;
    for (; (fen[i] != ' ' && fen[i] != '\0'); i++) {

        board->state.fullmoves = board->state.fullmoves * 10 + (fen[i] - '0');
    }

    updateTurn(board, false);
    return true;
}

bool _BoardLoadFEN(ChessBoard* board) {
    char fenString[BOARD_FEN_LENGTH] = { '\0' };

    FILE* fenFile = fopen("board_fen.data", "r");

    /* Se não existir o arquivo usa a FEN padrão */
    if (fenFile == NULL)
        return BoardLoadFEN(board, BOARD_FEN_DEFAULT);

    /* Usa a FEN padrão caso não seja possível ler a FEN do arquivo */
    if (fgets(fenString, sizeof fenString, fenFile) == NULL) {
        fclose(fenFile);
        return BoardLoadFEN(board, BOARD_FEN_DEFAULT);
    }

    fclose(fenFile);

    /* Se a FEN lida do arquivo for uma FEN válida então tudo funciona */
    if (BoardLoadFEN(board, fenString))
        return true;

    /* Se não então FEN padrão entra em ação de novo */
    return BoardLoadFEN(board, BOARD_FEN_DEFAULT);
}


void BoardGetAsFEN(ChessBoard* board, char fenString[100]) {


    const char pieces_data[6] = {
        'k', 'q', 'b', 'n', 'r', 'p',
    };

    const char castling_data[8][3] = {
        "", "K", "Q", "KQ",
        "", "k", "q", "kq",
    };

    int square;
    int fenStringFile;
    char pieceSymbol;

    int strIndex = 0;


    for (int rank = 0; rank < 8; rank++) {
        fenStringFile = 0;

        for (int file = 0; file < 8; file++, fenStringFile++) {
            square = PieceSquare(rank, file);

            if (board->squares[square] == PIECE_NONE)
                continue;

            pieceSymbol = pieces_data[PieceGetType(board->squares[square]) - 1];
            if (PieceGetColor(board->squares[square]) == PIECE_WHITE)
                pieceSymbol = toupper(pieceSymbol);

            if (fenStringFile > 0)
                strIndex += snprintf(fenString+strIndex,100-strIndex, "%d", fenStringFile);
            strIndex += snprintf(fenString + strIndex, 100 - strIndex, "%c", pieceSymbol);

            fenStringFile = -1;
        }

        if (rank == 7)
            continue;

        if (fenStringFile > 0)
            strIndex += snprintf(fenString + strIndex, 100 - strIndex, "%d", fenStringFile);
        strIndex += snprintf(fenString + strIndex, 100 - strIndex, "%c", '/');
    }

    strIndex += snprintf(fenString + strIndex, 100 - strIndex, " %c", board->state.whoMoves == PIECE_WHITE ? 'w' : 'b');

    if (board->state.castlingWhite == 0 && board->state.castlingBlack == 0)
        strIndex += snprintf(fenString + strIndex, 100 - strIndex, "%s", " -");
    else
        strIndex += snprintf(fenString + strIndex, 100 - strIndex, " %s%s",
            castling_data[board->state.castlingWhite],
            castling_data[board->state.castlingBlack + 4]);

    if (board->state.enPassantSquare == 0)
        strIndex += snprintf(fenString + strIndex, 100 - strIndex, "%s", " -");
    else
        strIndex += snprintf(fenString + strIndex, 100 - strIndex, " %c%d",
            PieceFile(board->state.enPassantSquare) + 'a',
            8 - PieceRank(board->state.enPassantSquare));

    strIndex += snprintf(fenString + strIndex, 100 - strIndex, " %d %d", board->state.halfmoves, board->state.fullmoves);

}





bool BoardSaveFEN(ChessBoard* board) {
    FILE* fenFile = fopen("board_fen.data", "w");

    if (fenFile == NULL)
        return false;

    char fenString[100];
    BoardGetAsFEN(board, fenString);

    fprintf(fenFile, fenString);
    fclose(fenFile);

    return true;
}

void BoardUnload(Board* board) {
    UnloadTexture(board->pieceSpriteSheet);
}

void BoardResize(Board* board, int screenWidth, int screenHeight) {
    board->squareLength = (screenWidth < screenHeight ? screenWidth : screenHeight)
        / BOARD_SQUARE_LENGTH_FACTOR;

    board->drawPosition.x = (screenWidth / 2.f) - (board->squareLength * 4);
    board->drawPosition.y = (screenHeight / 2.f) - (board->squareLength * 4);
}


bool BoardMakeMove(ChessBoard* board, int from, int to, int extra, bool updateWhoMoves) {
    if (board->move.list[from][to] == MOVE_NONE || from == to)
        return false;
         
    const int enPassantSquare = board->state.enPassantSquare
        + (PieceHasColor(board->squares[from], PIECE_BLACK) ? -8 : 8);

    int castlingSide;

    if (updateWhoMoves) {
        board->state.halfmoves++;

        if (!PieceHasType(board->squares[to], PIECE_NONE) || PieceHasType(board->squares[from], PIECE_PAWN)) {
            board->state.halfmoves = 0;
        }
    }

    board->state.enPassantSquare = 0;
    switch (board->move.list[from][to]) {
    case MOVE_PAWN_EN_PASSANT:
        board->squares[enPassantSquare] = PIECE_NONE;
        board->squares[to] = board->squares[from];

        break;

    case MOVE_PAWN_TWO_FORWARD:
        board->state.enPassantSquare = to + (PieceHasColor(board->squares[from], PIECE_BLACK) ? -8 : 8);
        /* fall through */

    case MOVE_NORMAL:
        board->squares[to] = board->squares[from];
        break;

    case MOVE_PAWN_PROMOTE:

        board->squares[to] = extra + PieceGetColor(board->squares[from]);

        break;

    case MOVE_CASTLING_KING:
        board->squares[to] = board->squares[from];
        board->squares[from + 1] = board->squares[from + 3];
        board->squares[from + 3] = PIECE_NONE;
        break;

    case MOVE_CASTLING_QUEEN:
        board->squares[to] = board->squares[from];
        board->squares[from - 1] = board->squares[from - 4];
        board->squares[from - 4] = PIECE_NONE;
        break;

    default:
        break;
    }

    /* Disable any castling because the kings has moved */
    if (PieceHasType(board->squares[from], PIECE_KING)) {
        if (PieceHasColor(board->squares[from], PIECE_WHITE))
            board->state.castlingWhite = CASTLING_NONE;
        else
            board->state.castlingBlack = CASTLING_NONE;
        /* Disable one side castling because the rook has moved */
    }
    else if (PieceHasType(board->squares[from], PIECE_ROOK)) {
        /* Queen side rooks */
        if (from == 56 || from == 0)
            castlingSide = CASTLING_KING_SIDE;
        /* King side rooks */
        else if (from == 63 || from == 7)
            castlingSide = CASTLING_QUEEN_SIDE;
        else
            castlingSide = CASTLING_NONE;

        if (PieceHasColor(board->squares[from], PIECE_WHITE))
            board->state.castlingWhite &= castlingSide;
        else
            board->state.castlingBlack &= castlingSide;
    }

    board->squares[from] = PIECE_NONE;
    if (updateWhoMoves)
        updateTurn(board, true);

    return true;
}

bool BoardKingInCheck(ChessBoard* board, int kingColor) { 

    // board.kingSquare[WhiteKing square, BlackKing square]
    return (board->move.attackSquares[board->kingSquare[kingColor != PIECE_WHITE]]) && board->state.whoMoves == kingColor;
}

bool BoardKingInMate(ChessBoard* board, int kingColor) {
    if (BoardKingInCheck(board, kingColor) && board->move.count == 0)
        return true;

    return false;
}

int BoardPerft(Board * board, int depth) {
    int nodes = 0;

#ifdef BOARD_PERFT
#define THREADS_NUMBER 6
    if (depth == 1)
        return board->move.count;
    else
        /* Because of the next loop the depth is one more than the passed to the
         * function, therefore, to get the correct depth subtract 1 from it.
         */
        depth--;

    PerftData datas[THREADS_NUMBER];
    int thr;

    for (int move = 0; move < 4096; ) {
        for (thr = 0; thr < THREADS_NUMBER && move < 4096; thr++, move++) {
            memcpy(&datas[thr].board, &board, sizeof(board));

            if (!BoardMakeMove(datas[thr].board, move / 64, move % 64, true)) {
                thr--;
                continue;
            }

            datas[thr].depth = depth;
            datas[thr].nodes = 0;

            if (thrd_create(&datas[thr].thrd, (thrd_start_t)perft, &datas[thr]) == thrd_error)
                thr--;
        }

        for (; thr > 0; thr--) {
            thrd_join(datas[thr - 1].thrd, NULL);
            nodes += datas[thr - 1].nodes;
        }
    }
#undef THREADS_NUMBER
#endif /* BOARD_PERFT */

    return nodes;
}

void BoardMakeMoveHandler(Board* board, int from, int to, int promotion) {
    int moveType = board->chessBoard.move.list[from][to];
    bool sucesso = BoardMakeMove(&board->chessBoard, from, to, 0, true);

    

    if (sucesso) {
        board->updated = true;

        // Tocar som?
        printf("Movimento %d\n", moveType);
        PlaySound(sndCapture);
    }
    
}


void BoardUpdate(Board* board) {

    // Game State
    if(board->gameState == GAME_PLAYING){
        if (BoardKingInMate(&board->chessBoard, PIECE_WHITE)) {
            board->gameState = GAME_BLACKWINS;
        }
        else if (BoardKingInMate(&board->chessBoard, PIECE_BLACK)) {
            board->gameState = GAME_WHITEWINS;
        }
        else if (boardInDraw(&board->chessBoard)) {
            board->gameState = GAME_DRAW;
        }
    }


    // Inputs
    if (IsKeyPressed(KEY_SPACE)) {
        board->xRotating = true;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        board->prevFenIndex = min(board->prevFenIndex + 1, board->prevFenTotal-1);
        BoardLoadFEN(&board->displayChessBoard, board->prevFen[board->prevFenIndex]);
    }

    if (IsKeyPressed(KEY_LEFT)) {
        board->prevFenIndex = max(board->prevFenIndex - 1, 0);
        BoardLoadFEN(&board->displayChessBoard, board->prevFen[board->prevFenIndex]);
    }


    // Promotion Menu
    if (board->promotion.active) {
        updatePromotionMenu(board);
    }
    else {
        // Dragging Pieces and Making Moves
        if (board->prevFenIndex == board->prevFenTotal - 1 && !board->xRotating && !board->backButtonClicked) {
            BoardPieceUpdate(board);
        }
    }


    // SINGLEPLAYER Bot
    if (isSinglePlayer && board->gameState == GAME_PLAYING) {
        BoardBotUpdate(board);
    }


    // Updating Display Chess Board
    if (board->updated) {
        board->updated = false;

        BoardGetAsFEN(&board->chessBoard, board->prevFen[board->prevFenTotal]);
        board->prevFenTotal++;
        board->prevFenIndex = board->prevFenTotal - 1;

        BoardLoadFEN(&board->displayChessBoard, board->prevFen[board->prevFenIndex]);
    }



    // Animando a troca de perspectiva girando o tabuleiro
    if (board->xRotating) {
        board->xAngle += 450 * GetFrameTime();
        
        board->yScale = cos(board->xAngle * DEG2RAD);

        if (board->xAngle >= 180) {
            board->xAngle = 0;
            board->viewAsWhite = !board->viewAsWhite;
            board->xRotating = false;
            board->yScale = 1;
        }
    }

    // Animando o anel de selecionar
    if (board->movingPiece.dragging || board->movingPiece.selecting)
        board->movingPiece.ringRotation += 150 * GetFrameTime();
    else
        board->movingPiece.ringRotation = 0;

    if (board->movingPiece.ringRotation > 360)
        board->movingPiece.ringRotation = 0;


    /* Resize the board if screen size has changed */
    if (IsWindowResized()) {
        BoardResize(board, GetScreenWidth(), GetScreenHeight());
    }
}


void BoardBotUpdate(Board* board) {
    if (board->chessBoard.state.whoMoves != saxaColor) return;

    if (saxaThinking) {

        if (!threadMoveData.finished) return;

        WaitForSingleObject(saxaMoveThreadId, INFINITE);
        CloseHandle(saxaMoveThreadId);

        saxa_move Move = threadMoveData.move;

        if (Move.from != -1 && Move.to != -1) {
            BoardMakeMoveHandler(board, Move.from, Move.to, Move.extra); 
        }
        else {
            printf("FreshFish is out of moves\n");
        }
        saxaThinking = false;
        
    }
    else {
        threadMoveData.board = board->chessBoard;
        threadMoveData.finished = false;
        threadMoveData.depth = saxaDephtBoard;
        //threadMoveData.depth = 5;
        saxaMoveThreadId = CreateThread(NULL, 0, backtrackingMoveThreaded, &threadMoveData, 0, NULL);

        if (saxaMoveThreadId != NULL) {
            saxaThinking = true;
        }
    }

    
}

void BoardPieceUpdate(Board* board) {
    int rank = (GetMouseY() - board->drawPosition.y) / board->squareLength;
    int file = (GetMouseX() - board->drawPosition.x) / board->squareLength;
    if (!board->viewAsWhite) {
        rank = 7 - rank;
    }


    int square = PieceSquare(rank, file);
    static bool clicked = false;

    if (rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            clicked = true;

            if (!board->movingPiece.dragging
                && !PieceHasType(board->chessBoard.squares[square], PIECE_NONE)
                && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves))
                board->movingPiece.position = square;


            if (clicked && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves)) {
                board->movingPiece.dragging = true;
            }


        }
        else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
            if ((board->movingPiece.dragging
                || (board->movingPiece.selecting
                    && board->movingPiece.position != square))
                && clicked) {
                board->movingPiece.dragging = false;
                board->movingPiece.selecting = false;

                int pieceFrom = board->movingPiece.position;
                int pieceTo = square;

                if (board->chessBoard.state.whoMoves != saxaColor || isSinglePlayer == false) {
                    if (board->chessBoard.move.list[pieceFrom][pieceTo] != MOVE_NONE) {
                        if (board->chessBoard.move.list[pieceFrom][pieceTo] == MOVE_PAWN_PROMOTE) {
                            board->promotion.active = true;
                            board->promotion.from = pieceFrom;
                            board->promotion.to = pieceTo;
                        }
                        else {
                            BoardMakeMoveHandler(board, pieceFrom, pieceTo, 0);
                        }
                    }
                }
            }

            if (clicked && !board->movingPiece.selecting
                && !PieceHasType(board->chessBoard.squares[square], PIECE_NONE)
                && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves)) {
                board->movingPiece.position = square;

                board->movingPiece.selecting = true;
                board->movingPiece.dragging = false;
            }

            clicked = false;
        }
    }
    else {
        board->movingPiece.dragging = false;
        board->movingPiece.selecting = false;
    }
}
        


void updatePieceButtons(Board* board) {
    

    float sqrHei = board->squareLength * 8 / 6;

    Rectangle square = {
        board->drawPosition.x ,
        board->drawPosition.y,
        sqrHei,
        sqrHei
    };

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 6; j++) {
                square.x = board->drawPosition.x - sqrHei * 2 + square.width * i;
                square.y = board->drawPosition.y + square.height * j;

                if (CheckCollisionPointRec(GetMousePosition(), square)) {
                    board->editor.pieceType = (j + 1) + (i + 1) * PIECE_BLACK;
                }
            }
        }
    }
}

void drawPieceButtons(Board* board) {
    float sqrHei = board->squareLength * 8 / 6;

    Rectangle square = {
        board->drawPosition.x ,
        board->drawPosition.y,
        sqrHei,
        sqrHei
    };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            square.x = board->drawPosition.x - sqrHei * 2 + square.width * i;
            square.y = board->drawPosition.y + square.height * j;

            int piece = (j + 1) + (i + 1) * PIECE_BLACK;



            drawSquare(*board, square, (i+j)%2 ? WHITE : GRAY);
            drawPiece(*board, square, WHITE, piece, false);

            if (CheckCollisionPointRec(GetMousePosition(), square)) {
                drawSelectRing(*board, square.x + square.width / 2, square.y + square.height / 2, square.width * 0.5, (i + j) % 2 ? GRAY : WHITE);
            }
        }
    }
}





void drawBoardBackground(Board * board) {
    const float screenHeight = GetScreenHeight();
    const float screenwidth = GetScreenWidth();

    const Color bgInformationColor = {22,40,46,255};
    const Color bgColor = { 20,20,20,255 };

    const Rectangle bg = { 0,0,screenwidth,screenHeight };
    const Rectangle bgInformation = { board->drawPosition.x + board->squareLength * 8.5
        , screenHeight/5, screenwidth/5,screenHeight/1.5 };

    DrawRectangleRec(bg, bgColor);



    DrawRectangleRec(bgInformation, bgInformationColor);
    DrawTexture(bgLeft,0,0,WHITE);
    
    char fenString[100];
    BoardGetAsFEN(&board->chessBoard, fenString);

    DrawText(fenString, bgInformation.x, bgInformation.y, 10, WHITE);

    char movesString[100];
    sprintf(movesString, "%d", board->chessBoard.state.fullmoves);
    DrawText(movesString, bgInformation.x, bgInformation.y + 20, 10, WHITE);

    char hmovesString[100];
    sprintf(hmovesString, "%d", board->chessBoard.state.halfmoves);
    DrawText(hmovesString, bgInformation.x, bgInformation.y + 40, 10, WHITE);
    
}

void drawEvaluationBar(Board* board) {
    Rectangle barContainerRect = {
       board->drawPosition.x - 40,
       board->drawPosition.y,

       20,
       board->squareLength*8
    };

   Rectangle barRect = {
       board->drawPosition.x - 40,
       board->drawPosition.y,

       20,
       (board->squareLength*8*board->positionGrade)
    };

    DrawRectangleRec(barContainerRect, GRAY);
    DrawRectangleRec(barRect, WHITE);

}

void BoardDraw(Board * board, int * menu) {
    Rectangle squarePosition = {
        board->drawPosition.x + board->squareLength*4,
        board->drawPosition.y + board->squareLength*4,

        board->squareLength,
        board->squareLength
    };

    Color colors[11] = { PINK,SKYBLUE,PURPLE,GREEN,
                        GOLD,RED,MAROON,LIME,BEIGE,BLACK,MAGENTA
    };
    drawBoardBackground(board);
    drawEvaluationBar(board);
    Color color, opositeColor;

    for (int square = 0; square < 64; square++) {

        int drawRank = PieceRank(square);
        if (!board->viewAsWhite) {
            drawRank = 7 - drawRank;
        }

        float boardCenterX = (board->drawPosition.x + board->squareLength * 4);
        float boardCenterY = (board->drawPosition.y + board->squareLength * 4);

        squarePosition.x = boardCenterX  + board->squareLength * board->xScale * (PieceFile(square)-4);
        squarePosition.y = boardCenterY + board->squareLength * board->yScale * (drawRank-4);
        squarePosition.width = abs(board->squareLength*board->xScale);
        squarePosition.height = abs(board->squareLength*board->yScale);

        if (board->yScale < 0) {
            squarePosition.y += squarePosition.height * board->yScale;
        }
        if (board->xScale < 0) {
            squarePosition.x += squarePosition.width * board->xScale;
        }

     

        


        if ((drawRank + PieceFile(square)) % 2 == board->viewAsWhite) {


            if (selectionB == 0) {
                color = board->squareBlackColor;
            }
            else {
                color = colors[selectionB];
            }

            opositeColor = board->squareWhiteColor;

        }
        else {
            color = board->squareWhiteColor;
            opositeColor = board->squareBlackColor;
        }

        int piece = board->displayChessBoard.squares[square];

        bool selected = (board->movingPiece.selecting) && board->movingPiece.position == square;
        bool dragged  = (board->movingPiece.dragging ) && board->movingPiece.position == square;
        bool inCheck  = (PieceGetType(piece) == PIECE_KING) && BoardKingInCheck(&board->displayChessBoard, PieceGetColor(piece));
        

        /* Draw the square */
        drawSquare(*board, squarePosition, color);

        /* Draws king above red square if in check */
        if (inCheck) {
            DrawRectangleRec(squarePosition, RED);
        }

        /* Draw the piece, if it's a valid piece and isn't dragging it */
        if (!dragged) {
            drawPiece(*board, squarePosition, opositeColor, piece, selected);
        }

        /* Draws the Selection ring */
        if (selected) {
            drawSelectRing(*board, squarePosition.x + squarePosition.width/2, squarePosition.y +squarePosition.height / 2, squarePosition.width*0.4, opositeColor);
        }

        /* Draw the rank and the file on the board */
        drawRankAndFiles(*board, squarePosition, square, opositeColor);

        /* Draw the possible moves */
        drawMoves(*board, board->displayChessBoard, squarePosition, square);
    }

    if (!board->promotion.active)
        drawDraggingPiece(*board, board->displayChessBoard, squarePosition);

    drawMateWindow(board, menu);
    drawPromotionMenu(*board);
    backButton(board, menu);
}

static void generateMoves(ChessBoard* board, bool onlyLegalMoves) {

    findKings(board);


    bool legalMoves = false;

    /* Reset the moves list, setting all bytes to zero */
    memset(&board->move, 0, sizeof(board->move));

    memset(&board->pieceTypeNum, 0, sizeof(board->pieceTypeNum));

    int squareValue;

    for (int square = 0; square < 64; square++) {
        /* If it's generating for the board or "after king generation" */

        squareValue = square;

        /* If the piece in this square hasn't the color that's move this turn,
         * only generate pseudo-legal moves for it.
         */
        if (!PieceHasColor(board->squares[squareValue], board->state.whoMoves) || !onlyLegalMoves)
            legalMoves = false;
        /* If the piece has the same color that's move this turn but check if
         * it's needed to generate legal moves for it.
         */
        else if (onlyLegalMoves)
            legalMoves = true;
        else
            continue;

        int pieceType = PieceGetType(board->squares[squareValue]);

        if (pieceType != PIECE_KING && pieceType != PIECE_NONE) {
            board->pieceTypeNum[board->state.whoMoves / 8][pieceType - 2]++;
        }

        switch (pieceType) {
            
        case PIECE_QUEEN:
            
            generateSlightMoves(board, squareValue, legalMoves);
            break;
        case PIECE_BISHOP:
            generateSlightMoves(board, squareValue, legalMoves);
            break;
        case PIECE_ROOK:
            generateSlightMoves(board, squareValue, legalMoves);
            break;

        case PIECE_KNIGHT:
            generateKnightMoves(board, squareValue, legalMoves);
            break;

        case PIECE_PAWN:
            generatePawnMoves(board, squareValue, legalMoves);
            break;

        default:
            break;
        }

    }


    for (int i = 0; i < 2; i++) {
        if (!PieceHasColor(board->squares[board->kingSquare[i]], board->state.whoMoves) || !onlyLegalMoves)
            legalMoves = false;

        else if (onlyLegalMoves)
            legalMoves = true;

        generateKingMoves(board, board->kingSquare[i], legalMoves);
    }

}

static void findKings(ChessBoard* board) {
    int kingsFound = 0;
    for (int i = 0; i < 64; i++) {
        if (PieceGetType(board->squares[i]) == PIECE_KING) {
            kingsFound++;

            board->kingSquare[(PieceGetColor(board->squares[i]) == PIECE_BLACK)] = i;
               
            if (kingsFound >= 2) {
                return;
            }
        }
    }
}


static void generateKingMoves(ChessBoard* board, int square, bool legalMove) {

    static const int directionOffsets[8][2] = {
        {  0, -1 }, {  0,  1 },
        { -1, -1 }, { -1,  1 },
        {  1, -1 }, {  1,  1 },
        { -1,  0 }, {  1,  0 },
    };

    const bool castlingKing = (board->state.whoMoves == PIECE_WHITE ?
        board->state.castlingWhite : board->state.castlingBlack) & CASTLING_KING_SIDE;

    const bool castlingQueen = (board->state.whoMoves == PIECE_WHITE ?
        board->state.castlingWhite : board->state.castlingBlack) & CASTLING_QUEEN_SIDE;

    int targetSquare;
    int targetRank;
    int targetFile;

    int kingMoves;
    int castlingSide;

    for (int direction = 0; direction < 8; direction++) {
        /* If the king is in check can't do any cast this turn */
        if (BoardKingInCheck(board, PieceGetColor(board->squares[square])))
            kingMoves = 1;
        /* If the move is to left and can cast to queen side */
        else if (direction == 6 && castlingQueen)
            kingMoves = 2, castlingSide = MOVE_CASTLING_QUEEN;
        /* If the move is to right and can cast to king side */
        else if (direction == 7 && castlingKing)
            kingMoves = 2, castlingSide = MOVE_CASTLING_KING;
        else
            kingMoves = 1;

        for (int moves = 1; moves <= kingMoves; moves++) {
            targetRank = PieceRank(square) + directionOffsets[direction][1];
            targetFile = PieceFile(square) + directionOffsets[direction][0] * moves;

            targetSquare = PieceSquare(targetRank, targetFile);

            if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7
                || PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square])))
                break;

            if (!legalMove) {
                board->move.attackSquares[targetSquare] = true;
            }
            else if (isValidMove(*board, square, targetSquare)) {
                if (moves == 1) {
                    board->move.list[square][targetSquare] = MOVE_NORMAL;
                    board->move.count++;

                    /* This condition checks if the move is a capture of the
                     * king and in this case castling shouldn't be possible.
                     */
                    if (!PieceHasType(board->squares[targetSquare], PIECE_NONE))
                        break;
                    /* Can't castling on captures */
                }
                else if (PieceHasType(board->squares[targetSquare], PIECE_NONE)) {
                    board->move.list[square][targetSquare] = castlingSide;
                    board->move.count++;
                }
            }
            else {
                /* Always break the moves loop only, this stop the castling
                 * if doing one and stop the move loop in anyway.
                 */
                break;
            }
        }
    }
}

static void generateSlightMoves(ChessBoard * board, int square, bool legalMove) {
    static const int directionOffsets[8][2] = {
        {  0, -1 }, {  0,  1 },
        { -1,  0 }, {  1,  0 },
        {  1, -1 }, { -1,  1 },
        { -1, -1 }, {  1,  1 },
    };

    /* If it's bishop only use the up four offsets that's diagonal offsets */
    const int startDirection = PieceHasType(board->squares[square], PIECE_BISHOP) ? 4 : 0;

    /* If it's rook only use the bottom four offsets that's vertical and
     * horizontal offsets.
     */
    const int endDirection = PieceHasType(board->squares[square], PIECE_ROOK) ? 4 : 8;

    int targetSquare;
    int targetRank;
    int targetFile;

    for (int direction = startDirection; direction < endDirection; direction++) {
        /* Generate moves until reach some impossible place to move */
        for (int moves = 1;; moves++) {
            targetRank = PieceRank(square) + directionOffsets[direction][1] * moves;
            targetFile = PieceFile(square) + directionOffsets[direction][0] * moves;

            targetSquare = PieceSquare(targetRank, targetFile);

            if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7
                || PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square])))
                break;

            if (!legalMove) {
                board->move.attackSquares[targetSquare] = true;
            }
            else if (isValidMove(*board, square, targetSquare)) {
                board->move.list[square][targetSquare] = MOVE_NORMAL;
                board->move.count++;
            }

            if (!(PieceHasType(board->squares[targetSquare], PIECE_NONE)
                || PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square]))))
                break;
        }
    }
}

static void generateKnightMoves(ChessBoard * board, int square, bool legalMove) {
    static const int directionOffsets[8][2] = {
        { -1, -2 }, {  1, -2 },
        { -1,  2 }, {  1,  2 },
        { -2, -1 }, { -2,  1 },
        {  2, -1 }, {  2,  1 },
    };

    int targetSquare;
    int targetRank;
    int targetFile;

    for (int direction = 0; direction < 8; direction++) {
        targetRank = PieceRank(square) + directionOffsets[direction][1];
        targetFile = PieceFile(square) + directionOffsets[direction][0];

        targetSquare = PieceSquare(targetRank, targetFile);

        if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7
            || PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square])))
            continue;

        if (!legalMove) {
            board->move.attackSquares[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            board->move.list[square][targetSquare] = MOVE_NORMAL;
            board->move.count++;
        }
    }
}

static void generatePawnMoves(ChessBoard* board, int square, bool legalMove) {
    static const int directionOffsets[8][2] = {
        {  0,  1 },
        {  1,  1 },
        { -1,  1 },
    };

    /* If the piece is white the pawn moves up, i.e., subtracting the rank value
     * of the pawn, otherwise add the rank value
     */

    const int direction = PieceHasColor(board->squares[square], PIECE_WHITE) ? -1 : 1;

    /* If it's the first move of the pawn it can go two square forward */
    const int forwardMoves = (PieceRank(square) == 1) || (PieceRank(square) == 6) ? 2 : 1;

    int targetRank;
    int targetFile;
    int targetSquare;

    for (int moves = 1; moves <= forwardMoves; moves++) {
        targetRank = PieceRank(square) + directionOffsets[0][1] * moves * direction;
        targetFile = PieceFile(square) + directionOffsets[0][0];

        targetSquare = PieceSquare(targetRank, targetFile);

        if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7
            || !PieceHasType(board->squares[targetSquare], PIECE_NONE))
            break;

        if (!legalMove) {
            board->move.attackSquares[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            if (moves == 1) {
                if (targetSquare > 7 && targetSquare < 56) {
                    board->move.list[square][targetSquare] = MOVE_NORMAL;
                }
                else {
                    board->move.list[square][targetSquare] = MOVE_PAWN_PROMOTE;
                    board->move.promotionExtraCount += 3;
                }
            }
            else {
                board->move.list[square][targetSquare] = MOVE_PAWN_TWO_FORWARD;
            }

            board->move.count++;
        }
    }

    for (int moves = 1; moves <= 2; moves++) {
        targetRank = PieceRank(square) + directionOffsets[moves][1] * direction;
        targetFile = PieceFile(square) + directionOffsets[moves][0];

        targetSquare = PieceSquare(targetRank, targetFile);

        if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7
            || PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square])))
            continue;

        if (!legalMove) {
            board->move.attackSquares[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            if (targetSquare == board->state.enPassantSquare) {
                board->move.list[square][targetSquare] = MOVE_PAWN_EN_PASSANT;
                board->move.count++;
            }

            if (!PieceHasType(board->squares[targetSquare], PIECE_NONE)
                && !PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square]))) {
                if (targetSquare > 7 && targetSquare < 56){
                    board->move.list[square][targetSquare] = MOVE_NORMAL;
                }else{
                    board->move.list[square][targetSquare] = MOVE_PAWN_PROMOTE;
                    board->move.promotionExtraCount += 3;
                }
                board->move.count++;
            }
        }
    }
}

static void updateTurn(ChessBoard* board, bool updateWhoMove) {
    if (updateWhoMove) {
        if (board->state.whoMoves == PIECE_WHITE) {
            board->state.whoMoves = PIECE_BLACK;
        }
        else {
            board->state.whoMoves = PIECE_WHITE;
            board->state.fullmoves++;
        }
    }

    generateMoves(board, true);
}

static bool isValidMove(ChessBoard board, int from, int to) {
    board.move.list[from][to] = MOVE_NORMAL;

    BoardMakeMove(&board, from, to, 0, false);
    generateMoves(&board, false);
    return !BoardKingInCheck(&board, board.state.whoMoves);
}

static void updatePromotionMenu(Board* board) {
    static int promotionSelected = 5;


    const Rectangle menuRectangle = {
        board->drawPosition.x + board->squareLength,
        board->drawPosition.y + board->squareLength * 3,

        board->squareLength * 6,
        board->squareLength * 2,
    };

    Rectangle spritePosition = {
        menuRectangle.x + menuRectangle.width / 18.f,
        menuRectangle.y + menuRectangle.height / 7.f,

        menuRectangle.width / 4.5f,
        menuRectangle.height / 1.5f,
    };

    if (!board->promotion.active)
        return;

    for (int promotion = 1; promotion <= 4; promotion++) {
        if (!CheckCollisionPointRec(GetMousePosition(), spritePosition)) {
            spritePosition.x += spritePosition.width;
            continue;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && promotionSelected > 4) {
            promotionSelected = promotion;
        }
        else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && promotionSelected <= 4) {
            BoardMakeMoveHandler(board, board->promotion.from, board->promotion.to, promotion + 1);
            board->promotion.active = false;
            

            promotionSelected = 5;            
            break;
        }
    }
}

static void backButton(Board * board, int * menu) {
    static int clickOption = 0;
    static bool mouseOver = false;

    const Rectangle windowRectangle = {
        board->drawPosition.x + board->squareLength,
        board->drawPosition.y + board->squareLength * 2,

        board->squareLength * 6,
        board->squareLength * 4,
    };

    Rectangle backButtonRectangle = {
        0, 100,

        board->squareLength / 1.6f,
        board->squareLength / 1.6f,
    };

    Rectangle btnRectangle = {
        windowRectangle.x,
        windowRectangle.y + windowRectangle.height - windowRectangle.height / 5.f,

        0,
        windowRectangle.height / 7.f,
    };

    const int winTitleFontSize = windowRectangle.height / 8;
    int btnFontSize;

    const char* winTitleText = " Voltar ao menu? ";
    const int winTitleTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(winTitleText, winTitleFontSize) / 2.f;
    const int winTitleTextPosY = windowRectangle.y + winTitleFontSize / 2.f;

    const char* btnText[3] = { "Sim", "Salvar e voltar", "Não" };
    int btnTextPosX;
    int btnTextPosY;

    backButtonRectangle.x = GetScreenWidth() - backButtonRectangle.width - 100;

    DrawRectangleRounded(backButtonRectangle, 0.5f, 0, board->squareBlackColor);

    DrawTriangle((Vector2){
        backButtonRectangle.x + backButtonRectangle.width * 0.8f,
        backButtonRectangle.y * 1.08f
        }, (Vector2) {
            backButtonRectangle.x + backButtonRectangle.width * 0.2f,
            backButtonRectangle.y + backButtonRectangle.height / 2.f
        }, (Vector2) {
            backButtonRectangle.x + backButtonRectangle.width * 0.8f,
            backButtonRectangle.y + backButtonRectangle.height * 0.86f
        },
        board->squareWhiteColor);

    if (board->backButtonClicked) {
        DrawRectangleRounded(windowRectangle, 0.2f, 0, board->squareBlackColor);
        DrawRectangleRoundedLines(windowRectangle, 0.2f, 0, 3, board->squareWhiteColor);

        DrawText(winTitleText, winTitleTextPosX, winTitleTextPosY, winTitleFontSize, board->squareWhiteColor);

        for (int i = 0; i < 3; i++) {
            btnRectangle.width = windowRectangle.width * (mouseOver ? 0.6f : 0.8f);
            btnRectangle.x = windowRectangle.x + windowRectangle.width / 2.f - btnRectangle.width / 2.f;

            btnRectangle.y = windowRectangle.y + windowRectangle.height / 2.f - (2 - i * 2) * btnRectangle.height;

            if (CheckCollisionPointRec(GetMousePosition(), btnRectangle)) {
                mouseOver = true;

                if (clickOption == 0 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    clickOption = i + 1;
                }
                else if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
                    if (clickOption == i + 1 && clickOption <= 3) {

                        if (clickOption == 1) {
                            _BoardInit(board);
                            board->backButtonClicked = false;

                        }
                        else if (clickOption == 2) {
                            BoardSaveFEN(&board->chessBoard);
                            board->backButtonClicked = false;

                        }
                        else if (clickOption == 3) {
                            board->backButtonClicked = false;

                        }
                        if (clickOption != 3)
                            *menu = 0;
                    }

                    clickOption = 0;

                }
            }
            else {
                mouseOver = false;
            }

            btnRectangle.width = windowRectangle.width * (mouseOver ? 0.6f : 0.8f);
            btnRectangle.x = windowRectangle.x + windowRectangle.width / 2.f - btnRectangle.width / 2.f;

            btnFontSize = windowRectangle.height / (12 + mouseOver * 2);

            btnTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(btnText[i], btnFontSize) / 2.f;
            btnTextPosY = btnRectangle.y + btnRectangle.height / 2.f - btnFontSize / 2.f;

            DrawRectangleRounded(btnRectangle, 0.5f, 0, board->squareWhiteColor);
            DrawText(btnText[i], btnTextPosX, btnTextPosY, btnFontSize, board->squareBlackColor);
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), btnRectangle))
        mouseOver = true;
    else
        mouseOver = false;

    if (!board->backButtonClicked
        && CheckCollisionPointRec(GetMousePosition(), backButtonRectangle)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        board->backButtonClicked = !board->backButtonClicked;
}

static void drawSquare(Board board, Rectangle drawPosition, Color color) {
    if (CheckCollisionPointRec(GetMousePosition(), drawPosition)
        && !board.promotion.active)
        DrawRectangleRec(drawPosition, RED);
    else
        DrawRectangleRec(drawPosition, color);
}

static void drawPiece(Board board, Rectangle drawPosition, Color color, int piece, bool shrinked) {

    if (PieceHasType(piece, PIECE_NONE))
        return;

    Rectangle spritePosition = {
        0, 0,

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y
    };

    ///* Draw a red square if the king is in check */


    /* Draw the piece image if isn't dragging any piece or if is dragging
     * doesn't the piece is this square.
     */

    if (!shrinked) {
        spritePosition.x = spritePosition.width * (PieceGetType(piece) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(piece, PIECE_BLACK);

        DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2) { 0, 0 }, 0, WHITE);
    }
    else {
        drawPosition.x += drawPosition.width / 2.f;
        drawPosition.y += drawPosition.height / 2.f;

        drawPosition.width *= 0.7;
        drawPosition.height *= 0.7;

        drawPosition.x -= drawPosition.width / 2.f;
        drawPosition.y -= drawPosition.height / 2.f;

        spritePosition.x = spritePosition.width * (PieceGetType(piece) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(piece, PIECE_BLACK);

        DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2) { 0, 0 }, 0, WHITE);
        /* In here draw a replacement of the piece image that's dragging in this
         * case.
         */
    }
    
}

static void drawSelectRing(Board board, int centerX, int centerY, float size, Color color) {
    DrawRing((Vector2) { centerX, centerY}, size, size,
        board.movingPiece.ringRotation, 360 + board.movingPiece.ringRotation, 6, color);
}

static void drawRankAndFiles(Board board, Rectangle drawPosition, int square, Color color) {
    const int textFontSize = board.squareLength / 5;
    const int textSize = MeasureText("a", textFontSize);

    const int file = PieceFile(square);
    const int rank = PieceRank(square);

    if (rank == 7) {
        drawPosition.x += drawPosition.width - textSize - 5;
        drawPosition.y += drawPosition.height - textFontSize - 5;

        DrawText(TextFormat("%c", 'a' + file), drawPosition.x, drawPosition.y, textFontSize, color);
    }

    if (file == 0) {
        if (rank == 7) {
            drawPosition.x -= drawPosition.width - textSize - 5;
            drawPosition.y -= drawPosition.height - textFontSize - 5;
        }

        drawPosition.x += 5;
        drawPosition.y += 5;

        DrawText(TextFormat("%d", 8 - rank), drawPosition.x, drawPosition.y, textFontSize, color);
    }
}

static void drawMoves(Board board, ChessBoard chessBoard, Rectangle drawPosition, int square) {
    static const Color color = { 128, 128, 128, 100 };

    if ((!board.movingPiece.dragging && !board.movingPiece.selecting)
        || !chessBoard.move.list[board.movingPiece.position][square])
        return;

    drawPosition.x += drawPosition.width / 2.f;
    drawPosition.y += drawPosition.height / 2.f;

    if (PieceHasType(chessBoard.squares[square], PIECE_NONE))
        DrawCircle(drawPosition.x, drawPosition.y, drawPosition.width / 6.f, color);
    else
        DrawRing((Vector2){ drawPosition.x, drawPosition.y }, board.squareLength * 0.5,
            board.squareLength * 0.4, 0, 360, 0, color);
}

static void drawDraggingPiece(Board board, ChessBoard chessBoard, Rectangle drawPosition) {
    Rectangle spritePosition = {
        0, 0,

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y
    };

    if (!board.movingPiece.dragging)
        return;

    spritePosition.x = spritePosition.width * (PieceGetType(chessBoard.squares[board.movingPiece.position]) - 1);
    spritePosition.y = spritePosition.height * PieceHasColor(chessBoard.squares[board.movingPiece.position], PIECE_BLACK);

    drawPosition.x = GetMouseX() - board.squareLength / 2.f;
    drawPosition.y = GetMouseY() - board.squareLength / 2.f;

    DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2){ 0, 0 }, 0, WHITE);
}


int boardInDraw(ChessBoard * board) {
    // Regra dos 50 movimentos
    if (board->state.halfmoves >= 50) return 1;
   
    // Stalemate
    if (board->move.count <= 0 && !(BoardKingInMate(board,PIECE_BLACK) || BoardKingInMate(board,PIECE_WHITE))) {
        return 1;
    }

    // Material insuficiente
    //if(board->pieceTypeNum[][])
    // a implementar
    return 0;
}


static void drawMateWindow(Board * board, int * menu) {
    static bool click = false;
    static bool mouseOver = false;

    const Rectangle windowRectangle = {
        board->drawPosition.x + board->squareLength,
        board->drawPosition.y + board->squareLength * 2,

        board->squareLength * 6,
        board->squareLength * 4,
    };

    const Rectangle spriteRectangle = {
        board->pieceSpriteSize.x,
        board->pieceSpriteSize.y * (board->chessBoard.state.whoMoves != PIECE_BLACK),

        board->pieceSpriteSize.x,
        board->pieceSpriteSize.y,
    };

    const Rectangle spritePosition = {
        windowRectangle.x + windowRectangle.width * 2 / 5.f,
        windowRectangle.y + windowRectangle.height / 10.f,

        windowRectangle.width / 4.5f,
        windowRectangle.height / 3.f,
    };

    Rectangle btnRectangle = {
        windowRectangle.x,
        windowRectangle.y + windowRectangle.height - windowRectangle.height / 5.f,

        0,
        windowRectangle.height / 7.f,
    };

    const int wonFontSize = windowRectangle.height / 8;
    const int btnFontSize = windowRectangle.height / (12 + mouseOver * 2);

    char* wonText = board->chessBoard.state.whoMoves == PIECE_WHITE ? "PRETO VENCEU!" : "BRANCO VENCEU!";
    const int wonTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(wonText, wonFontSize) / 2.f;
    const int wonTextPosY = (spritePosition.y + spritePosition.height) * 1.004f;

    const char* btnText = "Voltar ao menu";
    const int btnTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(btnText, btnFontSize) / 2.f;
    const int btnTextPosY = btnRectangle.y + btnRectangle.height / 2.f - btnFontSize / 2.f;

    btnRectangle.width = MeasureText(btnText, btnFontSize) * 1.5f;
    btnRectangle.x += windowRectangle.width / 2.f - btnRectangle.width / 2.f;


    if (!boardInDraw(&board->chessBoard) && !BoardKingInMate(&board->chessBoard,board->chessBoard.state.whoMoves)){
        return;   
    }

    if (boardInDraw(&board->chessBoard)) {
        wonText = (char*)malloc(sizeof(char)* 15);
        strcpy(wonText, "STALEMATE KKK");
    }

    
   
    DrawRectangleRounded(windowRectangle, 0.2f, 0, board->squareBlackColor);
    DrawRectangleRoundedLines(windowRectangle, 0.2f, 0, 3, board->squareWhiteColor);

    DrawTexturePro(board->pieceSpriteSheet, spriteRectangle, spritePosition,
       (Vector2) { 0,0 }, 0, WHITE);
    DrawText(wonText, wonTextPosX, wonTextPosY, wonFontSize, board->squareWhiteColor);

    DrawRectangleRounded(btnRectangle, 0.5f, 0, board->squareWhiteColor);
    DrawText(btnText, btnTextPosX, btnTextPosY, btnFontSize, board->squareBlackColor);

    if (CheckCollisionPointRec(GetMousePosition(), btnRectangle))
        mouseOver = true;
    else
        mouseOver = false;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        click = false;

        if (CheckCollisionPointRec(GetMousePosition(), btnRectangle))
            click = true;
    }
    else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        if (click && CheckCollisionPointRec(GetMousePosition(), btnRectangle)) {
            *menu = 0;
            _BoardInit(board);
        }

        click = false;
    }
}

static void drawPromotionMenu(Board board) {
    const Rectangle menuRectangle = {
        board.drawPosition.x + board.squareLength,
        board.drawPosition.y + board.squareLength * 3,

        board.squareLength * 6,
        board.squareLength * 2,
    };

    Rectangle spriteRectangle = {
        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y * ((board.chessBoard.state.whoMoves == PIECE_BLACK)),

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y,
    };

    Rectangle spritePosition = {
        menuRectangle.x + menuRectangle.width / 18.f,
        menuRectangle.y + menuRectangle.height / 7.f,

        menuRectangle.width / 4.5f,
        menuRectangle.height / 1.5f,
    };

    if (!board.promotion.active)
        return;
    
    DrawRectangleRounded(menuRectangle, 0.2f, 0, board.squareBlackColor);
    DrawRectangleRoundedLines(menuRectangle, 0.2f, 0, 3, board.squareWhiteColor);

    for (int i = 1; i <= 4; i++) {
        if (CheckCollisionPointRec(GetMousePosition(), spritePosition))
            DrawRectangleRounded(spritePosition, 0.2f, 0, RED);

        DrawTexturePro(board.pieceSpriteSheet, spriteRectangle, spritePosition, (Vector2){ 0,0 }, 0, WHITE);

        spritePosition.x += spritePosition.width;
        spriteRectangle.x += spriteRectangle.width;
    }
}


