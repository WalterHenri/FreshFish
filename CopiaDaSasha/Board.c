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

Texture2D background;
Texture2D background2;
Texture2D logo;
Texture2D pecas;
Texture2D playHoverText;
Texture2D optionHoverText;
Texture2D sairHoverText;
Texture2D bgLeft;

Font titleFont;
Font optionsFont;

/* Internal helpers functions
 */
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
static void drawPiece(Board board, Rectangle drawPosition, int square, Color color);
static void drawRankAndFiles(Board board, Rectangle drawPosition, int square, Color color);
static void drawMoves(Board board, Rectangle drawPosition, int square);
static void drawDraggingPiece(Board board, Rectangle drawPosition);
static void drawMateWindow(Board* board, int* menu);
static void drawPromotionMenu(Board board);

void menuInit() {
    //upload das imagens usadas

    logo = LoadTexture("./assets/FreshFishLogo.png");
    pecas = LoadTexture("./assets/chess_pieces.png");
    background = LoadTexture("./assets/initialScreen.png");
    background2 = LoadTexture("./assets/menuScreen.png");
    titleFont = LoadFontEx("./Fonts/HipotesiS_Tittle.ttf", 256, 0, 0);
    optionsFont = LoadFontEx("./Fonts/LemonMilk_Content.otf",256,0,0);
    playHoverText = LoadTexture("./assets/chess-game.png");
    optionHoverText = LoadTexture("./assets/gear.png");
    bgLeft = LoadTexture("./assets/bg.png");
}

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

bool checkTextColision(Vector2 origin,Vector2 position,Font font,char* text,int fontSize,int spacing) {

    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    if ((origin.x >= position.x && origin.x <= position.x + size.x)
        && (origin.y >= position.y- size.y/2 && origin.y <= position.y + size.y/2)) {
        return true;
    }
    else {
        return false;
    }
}



void menu(int* menuorboard) {
    /*
        usamos essas variaveis para pegar o tamanho da tela e baseamos
    o tamanho do texto das sprites e as posições nessas variáveis.
    */

    const float screenHeight = GetScreenHeight();
    const float screenwidth = GetScreenWidth();

    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background,0,0,WHITE);

    Vector2 positionTitle;
    const int spacing = 2;
    const int fontSizeTitle = screenHeight / 6;

    positionTitle.x = screenwidth / 2 - MeasureTextEx(titleFont,"Fresh Fish",fontSizeTitle,2).x / 2;
    positionTitle.y = screenHeight / 3;

    DrawTextEx(titleFont,"Fresh Fish", positionTitle, fontSizeTitle,spacing,WHITE);
    
    const int fontSizeOption = screenHeight / 20;
    Vector2 positionJogar;
    positionJogar.x = screenwidth / 2 - MeasureTextEx(optionsFont, "JOGAR", fontSizeOption, 2).x / 2;
    positionJogar.y = screenHeight / 2;

    DrawTextEx(optionsFont,"JOGAR",positionJogar,fontSizeOption,spacing, WHITE);

    Vector2 positionOpcoes;
    positionOpcoes.x = screenwidth / 2 - MeasureTextEx(optionsFont, "OPCOES", fontSizeOption, 2).x / 2;
    positionOpcoes.y = positionJogar.y + (MeasureTextEx(optionsFont, "JOGAR", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "OPCOES", positionOpcoes, fontSizeOption, spacing, WHITE);

    Vector2 positionSair;
    positionSair.x = screenwidth / 2 - MeasureTextEx(optionsFont, "SAIR", fontSizeOption, 2).x / 2;
    positionSair.y = positionOpcoes.y + (MeasureTextEx(optionsFont, "OPCOES", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "SAIR", positionSair, fontSizeOption, spacing, WHITE);

    
    
    if (checkTextColision(mousePoint,positionJogar,optionsFont,"JOGAR",fontSizeOption,spacing)) {
        DrawTextEx(optionsFont, "JOGAR", positionJogar, fontSizeOption, spacing, YELLOW);
    
        Vector2 newPosition = positionJogar;
        const double scale = 0.15;
        newPosition.x -= scale * playHoverText.width + spacing;
        DrawTextureEx(playHoverText, newPosition,0,scale,WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 3;
        }
    }

    if (checkTextColision(mousePoint, positionOpcoes, optionsFont, "OPCOES", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "OPCOES", positionOpcoes, fontSizeOption, spacing, YELLOW);

        Vector2 newPosition = positionOpcoes;
        const double scale = 0.1;
        newPosition.x -= scale * playHoverText.width + spacing * 4;
        DrawTextureEx(optionHoverText, newPosition, 0, scale, WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 6;
            selectionB = 0;
        }
    }

    if (checkTextColision(mousePoint, positionSair, optionsFont, "SAIR", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "SAIR", positionSair, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0))
            exit(0);
    }
    

    EndDrawing();
}

void GameBoard(int* menuorboard) {

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();

    float buttonWidth = screenwidth / 20;
    float buttonHeight = screenHeight / 14;

    float squareSize = screenHeight / 20;
    float squarex = screenwidth / 2 - squareSize * 4;
    float squarey = screenHeight / 2 - squareSize * 4;


    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();


    ClearBackground(GRAY);



    Rectangle previous = { screenwidth / 4 - buttonWidth,screenHeight / 2 - buttonHeight / 2,buttonWidth,buttonHeight };
    Rectangle next = { (screenwidth * 3) / 4 - buttonWidth / 2,screenHeight / 2 - buttonHeight / 2,buttonWidth,buttonHeight };
    Rectangle voltar = { screenwidth / 4 - buttonWidth,(screenHeight * 3) / 4,buttonWidth * 4,buttonHeight };



    DrawRectangleRec(previous, YELLOW);
    DrawText("<-", screenwidth / 4 - buttonWidth + screenwidth / 1000,
        screenHeight / 2 - buttonHeight / 2 + screenHeight / 1000,
        screenwidth / 20, BLACK);

    DrawRectangleRec(next, YELLOW);

    DrawText("->", (screenwidth * 3) / 4 - buttonWidth / 2 + screenwidth / 1000,
        screenHeight / 2 - buttonHeight / 2 + screenHeight / 1000,
        screenwidth / 20, BLACK);

    DrawRectangleRec(voltar, YELLOW);

    DrawText("voltar", screenwidth / 4, (screenHeight * 3) / 4, screenwidth / 30, BLACK);

    if (CheckCollisionPointRec(mousePoint, voltar)) {

        DrawText("voltar", screenwidth / 4, (screenHeight * 3) / 4, screenwidth / 30, WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 0;
        }
    }

    if (CheckCollisionPointRec(mousePoint, previous)) {

        DrawText("<-", screenwidth / 4 - buttonWidth + screenwidth / 1000,
            screenHeight / 2 - buttonHeight / 2 + screenHeight / 1000,
            screenwidth / 20, WHITE);

        if (IsMouseButtonPressed(0)) {
            if (selectionB > 0) {
                selectionB--;
            }
            else {
                selectionB = 10;
            }
        }
    }

    if (CheckCollisionPointRec(mousePoint, next)) {

        DrawText("->", (screenwidth * 3) / 4 - buttonWidth / 2 + screenwidth / 1000,
            screenHeight / 2 - buttonHeight / 2 + screenHeight / 1000,
            screenwidth / 20, WHITE);

        if (IsMouseButtonPressed(0))
            if (selectionB < 10) {
                selectionB++;
            }
            else {
                selectionB = 0;
            }
    }

    char cores[11][50] = { "rosinha", "azul","roxinho","verdinho",
                         "dourado","vermelho","vinho","esgoto","beeeje","escuro",
                         "rosa choque"
    };

    Color colors[11] = { PINK,SKYBLUE,PURPLE,GREEN,
                        GOLD,RED,MAROON,LIME,BEIGE,BLACK,MAGENTA
    };

    DrawText(cores[selectionB], screenwidth / 2 - screenwidth / 10,
        screenHeight / 10, screenwidth / 20, colors[selectionB]);



    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Color cor = ((i + j) % 2 == 0) ? WHITE : colors[selectionB];
            DrawRectangle(squarex + (j * squareSize), squarey + (i * squareSize), squareSize, squareSize, cor);
        }
    }

    EndDrawing();
}


void gamemode(int* menuorboard) {

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();

    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background2, 0, 0, WHITE);

    

    Vector2 positionTitle;
    int spacing = 2;
    int fontSizeTitle = screenHeight / 6;

    positionTitle.x = screenwidth / 2 - MeasureTextEx(titleFont, "Modo De Jogo", fontSizeTitle, 2).x / 2;
    positionTitle.y = screenHeight / 3;

    DrawTextEx(titleFont, "Modo De Jogo", positionTitle, fontSizeTitle, spacing, WHITE);

    int fontSizeOption = screenHeight / 20;
    Vector2 positionJogar;
    positionJogar.x = screenwidth / 2 - MeasureTextEx(optionsFont, "DOIS JOGADORES", fontSizeOption, 2).x / 2;
    positionJogar.y = screenHeight / 2;

    DrawTextEx(optionsFont, "DOIS JOGADORES", positionJogar, fontSizeOption, spacing, WHITE);

    Vector2 positionOpcoes;
    positionOpcoes.x = screenwidth / 2 - MeasureTextEx(optionsFont, "CONTRA O COMPUTADOR", fontSizeOption, 2).x / 2;
    positionOpcoes.y = positionJogar.y + (MeasureTextEx(optionsFont, "DOIS JOGADORES", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "CONTRA O COMPUTADOR", positionOpcoes, fontSizeOption, spacing, WHITE);

    Vector2 positionSair;
    positionSair.x = screenwidth / 2 - MeasureTextEx(optionsFont, "RETROCEDER", fontSizeOption, 2).x / 2;
    positionSair.y = positionOpcoes.y + (MeasureTextEx(optionsFont, "CONTRA O COMPUTADOR", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "RETROCEDER", positionSair, fontSizeOption, spacing, WHITE);


    if (checkTextColision(mousePoint, positionJogar, optionsFont, "DOIS JOGADORES", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "DOIS JOGADORES", positionJogar, fontSizeOption, spacing, YELLOW);

        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 0;
            *menuorboard = 1;
        }
    }

    if (checkTextColision(mousePoint, positionOpcoes, optionsFont, "CONTRA O COMPUTADOR", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "CONTRA O COMPUTADOR", positionOpcoes, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0)) {
            *menuorboard = 4;
        }
    }

    if (checkTextColision(mousePoint, positionSair, optionsFont, "RETROCEDER", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "RETROCEDER", positionSair, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0))
            *menuorboard = 0;
    }


    EndDrawing();

  

}

void gameDificult(int* menuorboard) {

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();

    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background2, 0, 0, WHITE);


    int spacing = 2;
    int fontSizeOption = screenHeight / 20;
    Vector2 positionJogar;
    positionJogar.x = screenwidth / 2 - MeasureTextEx(optionsFont, "FACIL", fontSizeOption, 2).x / 2;
    positionJogar.y = screenHeight / 3;

    DrawTextEx(optionsFont, "FACIL", positionJogar, fontSizeOption, spacing, WHITE);

    Vector2 positionOpcoes;
    positionOpcoes.x = screenwidth / 2 - MeasureTextEx(optionsFont, "INTERMEDIARIO", fontSizeOption, 2).x / 2;
    positionOpcoes.y = positionJogar.y + (MeasureTextEx(optionsFont, "FACIL", fontSizeOption, 2).y)*2;

    DrawTextEx(optionsFont, "INTERMEDIARIO", positionOpcoes, fontSizeOption, spacing, WHITE);

    Vector2 positionSair;
    positionSair.x = screenwidth / 2 - MeasureTextEx(optionsFont, "COMPLEXO", fontSizeOption, 2).x / 2;
    positionSair.y = positionOpcoes.y + (MeasureTextEx(optionsFont, "INTERMEDIARIO", fontSizeOption, 2).y * 2);

    DrawTextEx(optionsFont, "COMPLEXO", positionSair, fontSizeOption, spacing, WHITE);

    Vector2 positionSair2;
    positionSair2.x = screenwidth / 2 - MeasureTextEx(optionsFont, "RETROCEDER", fontSizeOption, 2).x / 2;
    positionSair2.y = positionSair.y + (MeasureTextEx(optionsFont, "INTERMEDIARIO", fontSizeOption, 2).y * 2);

    DrawTextEx(optionsFont, "RETROCEDER", positionSair2, fontSizeOption, spacing, WHITE);

    if (checkTextColision(mousePoint, positionJogar, optionsFont, "FACIL", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "FACIL", positionJogar, fontSizeOption, spacing, YELLOW);

        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 1;
            saxaDephtBoard = 1;
            *menuorboard = 1;
        }
    }

    if (checkTextColision(mousePoint, positionOpcoes, optionsFont, "INTERMEDIARIO", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "INTERMEDIARIO", positionOpcoes, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 1;
            saxaDephtBoard = 2;
            *menuorboard = 1;
        }
    }

    if (checkTextColision(mousePoint, positionSair, optionsFont, "COMPLEXO", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "COMPLEXO", positionSair, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0))
        {
            isSinglePlayer = 1;
            saxaDephtBoard = 3;
            *menuorboard = 1;
        }
    }

    if (checkTextColision(mousePoint, positionSair2, optionsFont, "RETROCEDER", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "RETROCEDER", positionSair2, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0))
        {
            *menuorboard = 3;
        }
    }

    EndDrawing();
}

void updateSetPosition(Board* board) {
#define CLICK_TIME 0.15
    static double clickTime = 0;
    static bool clicked = false;

    int rank;
    int file;
    int square;

    /* Resize the board if screen size has changed */
    if (IsWindowResized())
        BoardResize(board, GetScreenWidth(), GetScreenHeight());

    if (board->backButtonClicked) {
        // Essa parte está sendo implementada em backButton
    }
    else {
        rank = (GetMouseY() - board->drawPosition.y) / board->squareLength;
        file = (GetMouseX() - board->drawPosition.x) / board->squareLength;

        square = PieceSquare(rank, file);



        if (rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                clickTime = clickTime > 0 ? clickTime : GetTime();
                clicked = true;

                /* When the button down time is greater than the click time this
                    * means that isn't a click anymore, therefore, start to drag
                    * the piece.
                    */
                if (clicked && GetTime() - clickTime > CLICK_TIME
                    && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves)) {
                    board->movingPiece.dragging = true;
                    board->movingPiece.selecting = false;

                    clickTime = 0;
                }

                if (!board->movingPiece.dragging
                    && !PieceHasType(board->chessBoard.squares[square], PIECE_NONE)
                    && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves))
                    board->movingPiece.position = square;
            }
            else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
                if ((board->movingPiece.dragging
                    || (board->movingPiece.selecting
                        && board->movingPiece.position != square))
                    && clicked) {
                    board->movingPiece.dragging = false;
                    board->movingPiece.selecting = false;

                    board->chessBoard.squares[square] = board->chessBoard.squares[board->movingPiece.position];

                }

                if (clicked && GetTime() - clickTime <= CLICK_TIME
                    && !board->movingPiece.selecting
                    && !PieceHasType(board->chessBoard.squares[square], PIECE_NONE)
                    && PieceHasColor(board->chessBoard.squares[square], board->chessBoard.state.whoMoves)) {
                    board->movingPiece.position = square;

                    board->movingPiece.selecting = true;
                    board->movingPiece.dragging = false;
                }

                clicked = false;
                clickTime = 0;
            }
        }
        else {
            board->movingPiece.dragging = false;
            board->movingPiece.selecting = false;
        }

    }

    if (board->movingPiece.dragging || board->movingPiece.selecting)
        board->movingPiece.ringRotation += 150 * GetFrameTime();
    else
        board->movingPiece.ringRotation = 0;

    if (board->movingPiece.ringRotation > 360)
        board->movingPiece.ringRotation = 0;
#undef CLICK_TIME
}

void showFen(Board* board) {

}


void setupPosition(int* menu) {

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();


    Board* board = (Board *) malloc(sizeof(Board));
    
    BoardInit(screenWidth, screenHeight);

    //setup Position Mode is hard
    updateSetPosition(board);

    BoardDraw(board, menu);

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
  

    board.chessBoard.state.fullmoves = 1;
    board.viewAsWhite = true;

    BoardResize(&board, screenWidth, screenHeight);
    _BoardLoadFEN(&board.chessBoard);


    return board;
}

void _BoardInit(Board* board) {
    board->backButtonClicked = false;
    board->viewAsWhite = true;

    memset(&board->movingPiece, 0, sizeof board->movingPiece);
    memset(&board->chessBoard.move, 0, sizeof board->chessBoard.move);

    

    BoardLoadFEN(&board->chessBoard, BOARD_FEN_DEFAULT);
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

    i++;
    for (; fen[i] != ' '; i++)
        board->state.halfmoves = board->state.halfmoves * 10 + (fen[i] - '0');

    i++;
    for (; fen[i] != ' '; i++)
        board->state.fullmoves = board->state.fullmoves * 10 + (fen[i] - '0');

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

bool BoardSaveFEN(ChessBoard* board) {


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

    FILE* fenFile = fopen("board_fen.data", "w");

    if (fenFile == NULL)
        return false;

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
                fprintf(fenFile, "%d", fenStringFile);
            fprintf(fenFile, "%c", pieceSymbol);

            fenStringFile = -1;
        }

        if (rank == 7)
            continue;

        if (fenStringFile > 0)
            fprintf(fenFile, "%d", fenStringFile);
        fprintf(fenFile, "/");
    }

    fprintf(fenFile, " %c", board->state.whoMoves == PIECE_WHITE ? 'w' : 'b');

    if (board->state.castlingWhite == 0 && board->state.castlingBlack == 0)
        fprintf(fenFile, " -");
    else
        fprintf(fenFile, " %s%s",
            castling_data[board->state.castlingWhite],
            castling_data[board->state.castlingBlack + 4]);

    if (board->state.enPassantSquare == 0)
        fprintf(fenFile, " -");
    else
        fprintf(fenFile, " %c%d",
            PieceFile(board->state.enPassantSquare) + 'a',
            8 - PieceRank(board->state.enPassantSquare));

    fprintf(fenFile, " %d %d", board->state.halfmoves, board->state.fullmoves);
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

        if (!PieceHasType(board->squares[to], PIECE_NONE)
            && !PieceHasColor(board->squares[to], board->state.whoMoves))
            board->state.halfmoves = 0;
    }

    board->state.enPassantSquare = 0;
    switch (board->move.list[from][to]) {
    case MOVE_PAWN_EN_PASSANT:
        board->squares[enPassantSquare] = PIECE_NONE;
        board->squares[to] = board->squares[from];

        if (updateWhoMoves)
            board->state.halfmoves = 0;

        break;

    case MOVE_PAWN_TWO_FORWARD:
        board->state.enPassantSquare = to + (PieceHasColor(board->squares[from], PIECE_BLACK) ? -8 : 8);

        if (updateWhoMoves)
            board->state.halfmoves = 0;

        /* fall through */

    case MOVE_NORMAL:
        board->squares[to] = board->squares[from];
        break;

    case MOVE_PAWN_PROMOTE:
        /* The +1 ensures that has a promotion even if it's for a black piece
         * with the file 0.
         */
        board->squares[to] = extra + PieceGetColor(board->squares[from]);
        /*
        board->state.waitPromotion = PieceFile(to) + 1;

        if (PieceHasColor(board->squares[from], PIECE_WHITE))
            board->state.waitPromotion |= 0b1000;
        */
        if (updateWhoMoves)
            board->state.halfmoves = 0;

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
    for (int square = 0; square < 64; square++)
        if (board->move.attackSquares[square]
            && PieceHasType(board->squares[square], PIECE_KING)
            && PieceHasColor(board->squares[square], kingColor))
            return true;

    return false;
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

void BoardUpdate(Board* board) {
#define CLICK_TIME 0.15
    static double clickTime = 0;
    static bool clicked = false;

    int rank;
    int file;
    int square;

    bool gameEnded = false;


    if (IsKeyPressed(KEY_SPACE)) {
        board->viewAsWhite = !board->viewAsWhite;
    }


    /* Resize the board if screen size has changed */
    if (IsWindowResized())
        BoardResize(board, GetScreenWidth(), GetScreenHeight());

    // Promotion Menu
    if (board->promotion.active) {
        if(!isSinglePlayer)
            updatePromotionMenu(board);
        else if (isSinglePlayer && board->chessBoard.state.whoMoves != saxaColor) {
            updatePromotionMenu(board);
        }
    }
    else if (BoardKingInMate(&board->chessBoard, board->chessBoard.state.whoMoves)) {
        // Essa parte está sendo implementada em drawMateWindow
        gameEnded = true;
    }
    else if (BoardKingInMate(&board->chessBoard, board->chessBoard.state.whoMoves == PIECE_WHITE? PIECE_BLACK : PIECE_WHITE )) {
        // Essa parte está sendo implementada em drawMateWindow
        gameEnded = true;
    }
    else if (boardInDraw(&board->chessBoard)) {
       // gameEnded = true;
    }
    else if (board->backButtonClicked) {
        // Essa parte está sendo implementada em backButton
    }
    else {
        rank = (GetMouseY() - board->drawPosition.y) / board->squareLength;
        file = (GetMouseX() - board->drawPosition.x) / board->squareLength;
        if (!board->viewAsWhite) {
            rank = 7 - rank;
        }


        square = PieceSquare(rank, file);


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
                        if (board->chessBoard.move.list[pieceFrom][pieceTo] == true) {
                            if (board->chessBoard.move.list[pieceFrom][pieceTo] == MOVE_PAWN_PROMOTE) {
                                board->promotion.active = true;
                                board->promotion.from = pieceFrom;
                                board->promotion.to = pieceTo;
                            }
                            else {
                                BoardMakeMove(&board->chessBoard, pieceFrom, pieceTo, 0, true);
                                board->positionGradeDepth = 0;
                                testCalculationAbort = true;
                            }
                        }
                   }

                    /*
                    else {
                        if (board->chessBoard.move.list[pieceFrom][pieceTo] == MOVE_PAWN_PROMOTE) {
                            board->promotion.active = true;
                            board->promotion.from = pieceFrom;
                            board->promotion.to = pieceTo;
                        }
                        else {

                            board->preMove[0] = pieceFrom;
                            board->preMove[1] = pieceTo;
                            board->preMove[2] = 0;

                            board->preMoveStored = true;
                        }
                    }
                    */

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

    if (!isSinglePlayer) {
        if (saxaThinkingTest) {
            if (threadMoveDataTest.finished) {
                WaitForSingleObject(saxaMoveTestThreadId, INFINITE);
                CloseHandle(saxaMoveTestThreadId);

                saxa_move Move = threadMoveDataTest.move;

                if (Move.from != -1 && Move.to != -1) {
                    board->positionGrade = Move.grade;
                    board->positionGradeDepth++;
                }

                saxaThinkingTest = false;
                testCalculationAbort = false;
            }
        }
        else {
            if (board->positionGradeDepth <= 2) {
                threadMoveDataTest.board = board->chessBoard;
                threadMoveDataTest.finished = false;
                threadMoveDataTest.depth = board->positionGradeDepth;

                saxaMoveTestThreadId = CreateThread(NULL, 0, backtrackingMoveTestThreaded, &threadMoveDataTest, 0, NULL);

                if (saxaMoveTestThreadId != NULL) {
                    saxaThinkingTest = true;
                }
            }
        }

    }


    if (isSinglePlayer && !gameEnded) {


        if (board->chessBoard.state.whoMoves != saxaColor) {
            if (board->preMoveStored) {
                BoardMakeMove(&board->chessBoard, board->preMove[0], board->preMove[1], board->preMove[2], true);
                board->preMoveStored = false;
                board->positionGradeDepth = 0;
                testCalculationAbort = true;
            }
        }
        else {

            if (saxaThinking) {
                if (threadMoveData.finished) {
                    WaitForSingleObject(saxaMoveThreadId, INFINITE);
                    CloseHandle(saxaMoveThreadId);

                    saxa_move Move = threadMoveData.move;

                    if (Move.from != -1 && Move.to != -1) {
                        BoardMakeMove(&board->chessBoard, Move.from, Move.to, Move.extra, true);
                        board->positionGradeDepth = 0;
                        testCalculationAbort = true;
                    }
                    else {
                        printf("FreshFish is out of moves\n");
                    }
                    saxaThinking = false;
                }
            }
            else {
                threadMoveData.board = board->chessBoard;
                threadMoveData.finished = false;
                threadMoveData.depth = saxaDephtBoard;

                saxaMoveThreadId = CreateThread(NULL, 0, backtrackingMoveThreaded, &threadMoveData, 0, NULL);

                if (saxaMoveThreadId != NULL) {
                    saxaThinking = true;
                }
            }
            
           
        }
    }

    if (board->movingPiece.dragging || board->movingPiece.selecting)
        board->movingPiece.ringRotation += 150 * GetFrameTime();
    else
        board->movingPiece.ringRotation = 0;

    if (board->movingPiece.ringRotation > 360)
        board->movingPiece.ringRotation = 0;
#undef CLICK_TIME
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
        board->drawPosition.x,
        board->drawPosition.y,

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

        squarePosition.x = board->drawPosition.x + squarePosition.width * PieceFile(square);
        squarePosition.y = board->drawPosition.y + squarePosition.height * drawRank;

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

        /* Draw the square */
        drawSquare(*board, squarePosition, color);

        /* Draw the piece, if it's a valid piece and isn't dragging it
         */
        drawPiece(*board, squarePosition, square, opositeColor);

        /* Draw the rank and the file on the board */
        drawRankAndFiles(*board, squarePosition, square, opositeColor);

        /* Draw the possible moves */
        drawMoves(*board, squarePosition, square);
    }

    if (!board->chessBoard.state.waitPromotion)
        drawDraggingPiece(*board, squarePosition);

    drawMateWindow(board, menu);
    drawPromotionMenu(*board);
    backButton(board, menu);
}

static void generateMoves(ChessBoard* board, bool onlyLegalMoves) {

    bool legalMoves = false;

    /* Reset the moves list, setting all bytes to zero */
    memset(&board->move, 0, sizeof(board->move));

    int squareValue;
    int kingsSquareIndex = 0;
    int kingsSquare[2];

    for (int square = 0; square < (64 + kingsSquareIndex); square++) {
        /* If it's generating for the board or "after king generation" */
        squareValue = square < 64 ? square : kingsSquare[square - 64];

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

        switch (PieceGetType(board->squares[squareValue])) {
        case PIECE_KING:
            /* The king moves need to be generated after all other pieces move
             * generation because of the king castling that must now if it's in
             * check, this information only can be fetch after all move
             * generation.
             *
             * Therefore, we defer the king move generation to end of the
             * generation, this ensures the check informations was fetched.
             */
            if (square < 64)
                kingsSquare[kingsSquareIndex++] = square;
            else
                generateKingMoves(board, squareValue, legalMoves);

            break;

        case PIECE_QUEEN:
        case PIECE_BISHOP:
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
}

static void generateKingMoves(ChessBoard * board, int square, bool legalMove) {

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
    const int forwardMoves = PieceRank(square) == 1 || PieceRank(square) == 6 ? 2 : 1;

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
    ChessBoard copiedBoard;

    memcpy(&copiedBoard, &board, sizeof(board));
    copiedBoard.move.list[from][to] = MOVE_NORMAL;

    BoardMakeMove(&copiedBoard, from, to, 0, false);
    generateMoves(&copiedBoard, false);

    for (int square = 0; square < 64; square++)
        if (copiedBoard.move.attackSquares[square]
            && PieceHasType(copiedBoard.squares[square], PIECE_KING)
            && PieceHasColor(copiedBoard.squares[square], copiedBoard.state.whoMoves))
            return false;

    return true;
}

static void updatePromotionMenu(Board* board) {
    static int promotionSelected = 5;

    //const int rank = (board->chessBoard.state.waitPromotion & 0b1000) == 0 ? 7 : 0;
    //const int file = (board->chessBoard.state.waitPromotion - 1) & 0b0111;

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

    if (!board->chessBoard.state.waitPromotion && !board->promotion.active)
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


            /*
            if (isSinglePlayer && board->chessBoard.state.whoMoves == saxaColor) {
                board->preMove[0] = board->promotion.from;
                board->preMove[1] = board->promotion.to;
                board->preMove[2] = promotion + 1;

                board->preMoveStored = true;
            }
            else {
            */
                BoardMakeMove(&board->chessBoard, board->promotion.from, board->promotion.to, promotion + 1, true);
            //}
            board->promotion.active = false;
            board->positionGradeDepth = 0;
            testCalculationAbort = true;
            
            /*
            board->chessBoard.squares[PieceSquare(rank, file)] = promotionSelected + PIECE_KING;

            if (board->chessBoard.state.waitPromotion & 0b1000)
                board->chessBoard.squares[PieceSquare(rank, file)] += PIECE_WHITE;
            else
                board->chessBoard.squares[PieceSquare(rank, file)] += PIECE_BLACK;

            updateTurn(&board->chessBoard, false);
            */

            promotionSelected = 5;
            board->chessBoard.state.waitPromotion = 0;
            
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
        && !board.chessBoard.state.waitPromotion)
        DrawRectangleRec(drawPosition, RED);
    else
        DrawRectangleRec(drawPosition, color);
}

static void drawPiece(Board board, Rectangle drawPosition, int square, Color color) {
    Rectangle spritePosition = {
        0, 0,

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y
    };

    if (PieceHasType(board.chessBoard.squares[square], PIECE_NONE))
        return;

    /* Draw a red square if the king is in check */
    if (PieceHasType(board.chessBoard.squares[square], PIECE_KING)
        && BoardKingInCheck(&board.chessBoard, PieceGetColor(board.chessBoard.squares[square])))
        DrawRectangleRec(drawPosition, RED);

    /* Draw the piece image if isn't dragging any piece or if is dragging
     * doesn't the piece is this square.
     */
    if ((!board.movingPiece.dragging && !board.movingPiece.selecting)
        || board.movingPiece.position != square) {
        spritePosition.x = spritePosition.width * (PieceGetType(board.chessBoard.squares[square]) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(board.chessBoard.squares[square], PIECE_BLACK);

        DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2){ 0,0 }, 0, WHITE);
    }
    else if (!board.movingPiece.dragging && board.movingPiece.selecting) {
        drawPosition.x += drawPosition.width / 2.f;
        drawPosition.y += drawPosition.height / 2.f;

        DrawRing((Vector2){ drawPosition.x, drawPosition.y }, board.squareLength * 0.4, board.squareLength * 0.5,
            board.movingPiece.ringRotation, 360 + board.movingPiece.ringRotation, 6, color);

        drawPosition.width *= 0.7;
        drawPosition.height *= 0.7;

        drawPosition.x -= drawPosition.width / 2.f;
        drawPosition.y -= drawPosition.height / 2.f;

        spritePosition.x = spritePosition.width * (PieceGetType(board.chessBoard.squares[square]) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(board.chessBoard.squares[square], PIECE_BLACK);

        DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition,(Vector2){ 0,0 }, 0, WHITE);
        /* In here draw a replacement of the piece image that's dragging in this
         * case.
         */
    }
    else {
        drawPosition.x += drawPosition.width / 2.f;
        drawPosition.y += drawPosition.height / 2.f;

        DrawRing((Vector2){ drawPosition.x, drawPosition.y }, board.squareLength * 0.4, board.squareLength * 0.5,
            board.movingPiece.ringRotation, 360 + board.movingPiece.ringRotation, 6, color);
    }
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

static void drawMoves(Board board, Rectangle drawPosition, int square) {
    static const Color color = { 128, 128, 128, 100 };

    if ((!board.movingPiece.dragging && !board.movingPiece.selecting)
        || !board.chessBoard.move.list[board.movingPiece.position][square])
        return;

    drawPosition.x += drawPosition.width / 2.f;
    drawPosition.y += drawPosition.height / 2.f;

    if (PieceHasType(board.chessBoard.squares[square], PIECE_NONE))
        DrawCircle(drawPosition.x, drawPosition.y, drawPosition.width / 6.f, color);
    else
        DrawRing((Vector2){ drawPosition.x, drawPosition.y }, board.squareLength * 0.5,
            board.squareLength * 0.4, 0, 360, 0, color);
}

static void drawDraggingPiece(Board board, Rectangle drawPosition) {
    Rectangle spritePosition = {
        0, 0,

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y
    };

    if (!board.movingPiece.dragging)
        return;

    spritePosition.x = spritePosition.width * (PieceGetType(board.chessBoard.squares[board.movingPiece.position]) - 1);
    spritePosition.y = spritePosition.height * PieceHasColor(board.chessBoard.squares[board.movingPiece.position], PIECE_BLACK);

    drawPosition.x = GetMouseX() - board.squareLength / 2.f;
    drawPosition.y = GetMouseY() - board.squareLength / 2.f;

    DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2){ 0, 0 }, 0, WHITE);
}


int boardInDraw(ChessBoard * board) {

    if (board->move.count <= 0) {
        return 1;
    }
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
        board.pieceSpriteSize.y * ((board.chessBoard.state.waitPromotion & 0b1000) == 0),

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y,
    };

    Rectangle spritePosition = {
        menuRectangle.x + menuRectangle.width / 18.f,
        menuRectangle.y + menuRectangle.height / 7.f,

        menuRectangle.width / 4.5f,
        menuRectangle.height / 1.5f,
    };

    if (!board.chessBoard.state.waitPromotion && !board.promotion.active)
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


