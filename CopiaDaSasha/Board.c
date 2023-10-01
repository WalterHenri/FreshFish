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

Texture2D xuxinha;
Texture2D mao;
Texture2D logo;
Texture2D txutxucao;
Texture2D joseval;
Texture2D pecas;


/* Internal helpers functions
 */
static void generateMoves(Board* board, bool onlyLegalMoves);
static void updateTurn(Board* board, bool updateWhoMove);
static bool isValidMove(Board board, int from, int to);

/* Internal generate move functions
 * This functions generate moves for specific pieces
 */
static void generateKingMoves(Board* board, int square, bool legal);
static void generateSlightMoves(Board* board, int square, bool legal);
static void generateKnightMoves(Board* board, int square, bool legal);
static void generatePawnMoves(Board* board, int square, bool legal);

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
    xuxinha = LoadTexture("./assets/xuxinha.png");
    mao = LoadTexture("./assets/mao.png");
    logo = LoadTexture("./assets/logo.png");
    xuxinha = LoadTexture("./assets/xuxinha.png");
    txutxucao = LoadTexture("./assets/txutxucao ufs.png");
    joseval = LoadTexture("./assets/professorjoseval.png");
    pecas = LoadTexture("./assets/chess_pieces.png");

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

void menu(int* menuorboard) {
    /*
        usamos essas variaveis para pegar o tamanho da tela e baseamos
    o tamanho do texto das sprites e as posições nessas variáveis.
    */

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();

    float xOfButtons = screenwidth / 19;
    float YOfButtons = screenHeight / 2;

    float inBetween = screenHeight / 20;
    float inBetweenTextX = screenwidth / 14;
    float inBetweenTextY = screenHeight / 100;

    float buttonWidth = screenwidth / 5;
    float buttonHeight = screenHeight / 30;

    float sizeOftext = screenHeight / 50;

    float handX = screenwidth / 4;

    Vector2 mousePoint = GetMousePosition();

    Rectangle rec1 = { xOfButtons, YOfButtons, buttonWidth, buttonHeight };
    Rectangle rec2 = { xOfButtons, YOfButtons + inBetween, buttonWidth, buttonHeight };
    Rectangle rec3 = { xOfButtons, YOfButtons + (inBetween * 2), buttonWidth, buttonHeight };
    Rectangle rec4 = { xOfButtons, YOfButtons + (inBetween * 3), buttonWidth, buttonHeight };
    Rectangle rec5 = { xOfButtons, YOfButtons + (inBetween * 4), buttonWidth, buttonHeight };


    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(xuxinha, screenwidth / 2, screenHeight / 15, RAYWHITE);
    DrawTexture(logo, screenwidth / 25, screenHeight / 70, RAYWHITE);

    DrawRectangleLines(xOfButtons, YOfButtons, buttonWidth, buttonHeight, PINK);

    DrawText("Jogar", xOfButtons + inBetweenTextX, YOfButtons + inBetweenTextY, sizeOftext, WHITE);

    if (CheckCollisionPointRec(mousePoint, rec1)) {

        DrawTexture(mao, handX, YOfButtons, RAYWHITE);
        if (IsMouseButtonPressed(0)) {
            *menuorboard = 3;
        }
    }

    DrawRectangleLines(xOfButtons, YOfButtons + inBetween, buttonWidth, buttonHeight, PINK);

    DrawText("Tabuleiros", xOfButtons + inBetweenTextX,
        YOfButtons + inBetween + inBetweenTextY,
        sizeOftext, WHITE);

    if (CheckCollisionPointRec(mousePoint, rec2)) {

        DrawTexture(mao, handX, YOfButtons + inBetween, RAYWHITE);
        if (IsMouseButtonPressed(0)) {
            *menuorboard = 6;
            selectionB = 0;
        }
    }

    DrawRectangleLines(xOfButtons, YOfButtons + (inBetween * 2), buttonWidth, buttonHeight, PINK);

    DrawText("Como jogar?", xOfButtons + inBetweenTextX,
        YOfButtons + (inBetween * 2) + inBetweenTextY,
        sizeOftext, WHITE);

    if (CheckCollisionPointRec(mousePoint, rec3)) {

        DrawTexture(mao, handX, YOfButtons + (inBetween * 2), RAYWHITE);
        if (IsMouseButtonPressed(0)) {
            quadros = 1;
            *menuorboard = 2;
        }
    }

    DrawRectangleLines(xOfButtons, YOfButtons + (inBetween * 3), buttonWidth, buttonHeight, PINK);


    DrawText("Meu perfil", xOfButtons + inBetweenTextX,
        YOfButtons + (inBetween * 3) + inBetweenTextY,
        sizeOftext, WHITE
    );

    if (CheckCollisionPointRec(mousePoint, rec4)) {

        DrawTexture(mao, handX, YOfButtons + (inBetween * 3), RAYWHITE);
        if (IsMouseButtonPressed(0)) {
            counter = 0;
            counterR = 0;
            countere = 0;
            istexting = false;
            istextingr = false;
            istextinge = false;
            *menuorboard = 5;
        }
    }

    DrawRectangleLines(xOfButtons, YOfButtons + (inBetween * 4), buttonWidth, buttonHeight, PINK);
    DrawText("Sair", xOfButtons + inBetweenTextX,
        YOfButtons + (inBetween * 4) + inBetweenTextY,
        sizeOftext, WHITE
    );

    if (CheckCollisionPointRec(mousePoint, rec5)) {
        DrawTexture(mao, handX, YOfButtons + (inBetween * 4), RAYWHITE);
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

static void showLogin(Profile* player, int* menuorboard) {


    char rating1[50];

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();
    float buttonWidth = screenwidth / 5;
    float buttonHeight = screenHeight / 30;
    float sizeFont = screenHeight / 30;
    float xButtonSave = screenwidth / 4 + screenwidth / 4 + buttonWidth / 2;
    float yButtonSave = (3 * screenHeight) / 4;
    float umxai = screenwidth / 4 + screenwidth / 4 - buttonWidth / 2;
    float xtext = umxai + screenwidth / 300;
    float x = screenwidth / 4 + screenwidth / 100;
    float ytext = screenHeight / 3 + buttonHeight + screenHeight / 500;

    Rectangle saveRec = { xButtonSave,yButtonSave,buttonWidth / 2,buttonHeight * 2 };
    Rectangle retanguloGrande = { screenwidth / 4,screenHeight / 8,screenwidth / 2,(screenHeight * 3) / 4 };
    Rectangle voltar = { xButtonSave - buttonWidth * 1.5f,yButtonSave, buttonWidth / 2,buttonHeight * 2 };


    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRec(voltar, YELLOW);
    DrawRectangleRec(saveRec, YELLOW);
    DrawRectangleLinesEx(retanguloGrande, 2, BLUE);

    DrawText("voltar", xButtonSave - (buttonWidth * 1.4), yButtonSave + screenHeight / 50, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, voltar)) {

        DrawText("voltar", xButtonSave - (buttonWidth * 1.4), yButtonSave + screenHeight / 50, sizeFont, WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 0;
        }
    }

    DrawText(player->name, umxai, screenHeight / 5, sizeFont * 3, WHITE);

    myitoa(player->rating, rating1);

    DrawText("FIDE rating:", x, ytext + buttonHeight * 2, sizeFont, RED);
    DrawText(rating1, xtext, ytext + buttonHeight * 2, sizeFont * 2, RED);
    DrawText("Email:", x, ytext + buttonHeight * 6, sizeFont, YELLOW);
    DrawText(player->email, x + sizeFont * 5, ytext + buttonHeight * 6, sizeFont * 1.5, YELLOW);
    DrawText("Editar", xButtonSave + screenwidth / 50, yButtonSave + screenHeight / 50, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, saveRec)) {
        DrawText("Editar", xButtonSave + screenwidth / 50, yButtonSave + screenHeight / 50, sizeFont, WHITE);
        if (IsMouseButtonPressed(0)) {
            player->id = 0;
            player->isLogged = false;
        }
    }
}

void makeLogin(Profile* player, int* menuorboard) {

    /*daqui pra frente, veremos uma vasta quantidade de variaveis para definir tamanho e local de desenhar coisas na tela*/

    char rating[50];
    myitoa(player->rating, rating);

    float screenHeight = GetScreenHeight();
    float screenwidth = GetScreenWidth();

    float buttonWidth = screenwidth / 5;
    float buttonHeight = screenHeight / 30;


    float umxai = screenwidth / 4 + screenwidth / 4 - buttonWidth / 2;
    float xtext = umxai + screenwidth / 300;
    float x = screenwidth / 4 + screenwidth / 100;

    float xButtonSave = screenwidth / 4 + screenwidth / 4 + buttonWidth / 2;
    float yButtonSave = (3 * screenHeight) / 4;


    float ytext = screenHeight / 3 + buttonHeight + screenHeight / 500;

    Rectangle voltar = { xButtonSave - buttonWidth * 1.5f,yButtonSave, buttonWidth / 2,buttonHeight * 2 };
    Rectangle saveRec = { xButtonSave,yButtonSave,buttonWidth / 2,buttonHeight * 2 };

    Vector2 mousePoint = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRec(voltar, YELLOW);

    Rectangle retanguloGrande = { screenwidth / 4,screenHeight / 8,screenwidth / 2,(screenHeight * 3) / 4 };

    DrawRectangleLinesEx(retanguloGrande, 2, BLUE);

    float sizeFont = screenHeight / 30;

    DrawText("voltar", xButtonSave - (buttonWidth * 1.4), yButtonSave + screenHeight / 50, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, voltar)) {

        DrawText("voltar", xButtonSave - (buttonWidth * 1.4), yButtonSave + screenHeight / 50, sizeFont, WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 0;
        }
    }

    DrawText("clique para digitar e enter para confirmar",
        screenwidth / 4 + screenwidth / 200,
        screenHeight / 8 + screenHeight / 80
        , sizeFont, WHITE);

    Rectangle caixadetexto = {
        umxai,
        screenHeight / 3 + buttonHeight,
        buttonWidth,buttonHeight
    };

    DrawText("Nome:", umxai, screenHeight / 3, sizeFont, RED);
    DrawRectangleRec(caixadetexto, YELLOW);
    DrawText(player->name, xtext, ytext, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, caixadetexto)) {
        if (IsMouseButtonPressed(0)) {
            istexting = true;
            istextingr = false;
            istextinge = false;
        }
    }


    if (istexting) {

        DrawText("Seu apelido", x, ytext - buttonHeight * 4, sizeFont, WHITE);

        DrawRectangleRec(caixadetexto, WHITE);
        player->name[counter] = (char)GetCharPressed();

        if (IsKeyPressed(KEY_BACKSPACE)) {
            player->name[counter] = (char)0;
            counter -= 2;
            if (counter < 0) {
                player->name[0] = (char)0;
                counter = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {
            istexting = false;
        }
        if ((int)player->name[counter] != 0) {
            counter++;
        }

        DrawText(player->name, xtext, ytext, sizeFont, BLACK);

    }

    Rectangle caixadetextoR = {
        umxai,
        screenHeight / 3 + buttonHeight * 3,
        buttonWidth,buttonHeight
    };

    DrawText("Rating:", umxai, screenHeight / 3 + buttonHeight * 2, sizeFont, RED);
    DrawRectangleRec(caixadetextoR, YELLOW);
    DrawText(rating, xtext, ytext + buttonHeight * 2, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, caixadetextoR)) {
        if (IsMouseButtonPressed(0)) {
            istextingr = true;
            istexting = false;
            istextinge = false;
        }
    }

    if (istextingr) {

        DrawText("Seu FIDE rating", x, ytext - buttonHeight * 4, sizeFont, WHITE);
        DrawText("Caso não possua, digite 1000", x, ytext - buttonHeight * 3, sizeFont, WHITE);

        DrawRectangleRec(caixadetextoR, WHITE);

        rating[counterR] = (char)GetCharPressed();

        if ((rating[counterR] < 48 || rating[counterR] > 57) && (int)rating[counterR] != 0) {
            rating[counterR] = (char)0;
            counterR -= 2;
            if (counterR < 0) {
                rating[0] = (char)0;
                counterR = 0;
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            rating[counterR] = (char)0;
            counterR -= 2;
            if (counterR < 0) {
                rating[0] = (char)0;
                counterR = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {

            istextingr = false;

        }

        if ((int)rating[counterR] != 0) {
            counterR++;
        }
        player->rating = atoi(rating);
        DrawText(rating, xtext, ytext + buttonHeight * 2, sizeFont, BLACK);
    }


    Rectangle caixadetextoE = {
        umxai,
        screenHeight / 3 + buttonHeight * 5,
        buttonWidth,buttonHeight
    };

    DrawText("Email:", umxai, screenHeight / 3 + buttonHeight * 4, sizeFont, RED);
    DrawRectangleRec(caixadetextoE, YELLOW);
    DrawText(player->email, xtext, ytext + buttonHeight * 4, sizeFont, WHITE);

    if (CheckCollisionPointRec(mousePoint, caixadetextoE)) {
        if (IsMouseButtonPressed(0)) {
            istexting = false;
            istextingr = false;
            istextinge = true;
        }
    }


    if (istextinge) {

        DrawText("Seu endereço de email", x, ytext - buttonHeight * 4, sizeFont, WHITE);

        DrawRectangleRec(caixadetextoE, WHITE);
        player->email[countere] = (char)GetCharPressed();

        if (IsKeyPressed(KEY_BACKSPACE)) {
            player->email[countere] = (char)0;
            countere -= 2;
            if (countere < 0) {
                player->email[0] = (char)0;
                countere = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {
            istextinge = false;
        }
        if ((int)player->email[countere] != 0) {
            countere++;
        }

        DrawText(player->email, xtext, ytext + buttonHeight * 4, sizeFont, BLACK);
    }

    DrawRectangleRec(saveRec, YELLOW);


    DrawText("SALVAR", xButtonSave + screenwidth / 50, yButtonSave + screenHeight / 50, sizeFont, BLACK);

    if (CheckCollisionPointRec(mousePoint, saveRec)) {
        DrawText("SALVAR", xButtonSave + screenwidth / 50, yButtonSave + screenHeight / 50, sizeFont, WHITE);
        if (IsMouseButtonPressed(0)) {
            player->id = 1;
            player->isLogged = true;
            *menuorboard = 0;
        }

    }
}

static void createLoginFile() {

    /*criando um player "vazio"*/

    Profile player = { false,"",0,"",0 };

    const char* path = "login.txt";

    FILE* file = fopen(path, "wb");

    fwrite(&player, sizeof(player), 1, file);

    fclose(file);

}

void login(int* menuorboard) {

    /*guardamos as informações do player em um arquivo binário para ler depois
    as informações do usuário são guardadas em uma struct e a mesma escrita no arquivo no processo
    de leitura, ja no processo de saida os dados da struct sao lidas no arquivo
    tudo acontece nessa mesma função*/

    Profile player;
    const char* path = "login.txt";

    FILE* file;

    /*powered by Marcos Mota*/
    /*essa função cria um arquivo caso seja a primeira utilização*/
    if ((file = fopen(path, "rb+")) == NULL) {
        createLoginFile();
        file = fopen(path, "rb+");
    }

    fread(&player, sizeof(player), 1, file);

    /* a variavel islogged diz se o player vai para a  função de fazer
        o mostrar  o cadastro ou de fazer o cadastro.
        passamos o player  por referência em ambos ja que são essas funções
        que irão pegar a entrada do usuário*/

    if (player.isLogged)
        showLogin(&player, menuorboard);
    else
        makeLogin(&player, menuorboard);

    fclose(file);

    /*fechamos o arquivo e atualizamos com a struct que ja temos*/

    file = fopen(path, "wb+");

    fwrite(&player, sizeof(player), 1, file);

    fclose(file);
    EndDrawing();


}

void gamemode(int* menuorboard) {

    BeginDrawing(); // Comando para iniciar a desenhar
    ClearBackground(BLACK); //


    Vector2 mousePoint = GetMousePosition();

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();


    float botao1X = screenWidth / 2 - screenWidth / 8;

    float botao1Y = screenHeight / 4 - screenHeight / 8;

    float botao2Y = screenHeight / 4 + screenHeight / 8;

    float botao3Y = screenHeight / 4 + screenHeight / 8 * 3;

    int sizeOftext = screenHeight / 30;

    Rectangle botao1 = { botao1X, botao1Y, screenWidth / 4, screenHeight / 8 };
    Rectangle botao2 = { botao1X, botao2Y, screenWidth / 4, screenHeight / 8 };
    Rectangle botao3 = { botao1X, botao3Y, screenWidth / 4, screenHeight / 8 };

    DrawRectangleRounded(botao1, 0.5, sizeOftext, DARKGREEN);
    DrawText("Dois jogadores", botao1X + 30, botao1Y + 30, sizeOftext, WHITE);

    DrawRectangleRounded(botao2, 0.5, sizeOftext, DARKGREEN);
    DrawText("Contra o computador", botao1X + 30, botao2Y + 30, sizeOftext, WHITE);

    DrawRectangleRounded(botao3, 0.5, sizeOftext, DARKGREEN);
    DrawText("Voltar", botao1X + 30, botao3Y + 30, sizeOftext, WHITE);

    if (CheckCollisionPointRec(mousePoint, botao1)) {

        DrawRectangleRounded(botao1, 0.5, sizeOftext, RED);
        DrawText("Dois jogadores", botao1X + 30, botao1Y + 30, sizeOftext, WHITE);

        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 0;
            *menuorboard = 1;
        }
    }

    if (CheckCollisionPointRec(mousePoint, botao2)) {

        DrawRectangleRounded(botao2, 0.5, sizeOftext, GREEN);
        DrawText("Contra o computador", botao1X + 30, botao2Y + 30, sizeOftext, WHITE);
        if (IsMouseButtonPressed(0)) {

            *menuorboard = 4;
        }
    }

    if (CheckCollisionPointRec(mousePoint, botao3)) {

        DrawRectangleRounded(botao3, 0.5, sizeOftext, PINK);
        DrawText("Voltar", botao1X + 30, botao3Y + 30, sizeOftext, WHITE);
        if (IsMouseButtonPressed(0)) {
            *menuorboard = 0;
        }
    }


    EndDrawing();


}

void gameDificult(int* menuorboard) {

    BeginDrawing(); // Comando para iniciar a desenhar
    ClearBackground(BLACK); //


    Vector2 mousePoint = GetMousePosition();

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();


    float botao1X = screenWidth / 2 - screenWidth / 8;

    float botao1Y = screenHeight / 4 - screenHeight / 8;

    float botao2Y = screenHeight / 4 + screenHeight / 8;

    float botao3Y = screenHeight / 4 + screenHeight / 8 * 3;

    int sizeOftext = screenHeight / 30;

    Rectangle botao1 = { botao1X, botao1Y, screenWidth / 4, screenHeight / 8 };
    Rectangle botao2 = { botao1X, botao2Y, screenWidth / 4, screenHeight / 8 };
    Rectangle botao3 = { botao1X, botao3Y, screenWidth / 4, screenHeight / 8 };

    DrawRectangleRounded(botao1, 0.5, sizeOftext, DARKGREEN);
    DrawText("Fácil", botao1X + 30, botao1Y + 30, sizeOftext, WHITE);

    DrawRectangleRounded(botao2, 0.5, sizeOftext, DARKGREEN);
    DrawText("Médio", botao1X + 30, botao2Y + 30, sizeOftext, WHITE);

    DrawRectangleRounded(botao3, 0.5, sizeOftext, DARKGREEN);
    DrawText("Dificil", botao1X + 30, botao3Y + 30, sizeOftext, WHITE);

    if (CheckCollisionPointRec(mousePoint, botao1)) {

        DrawRectangleRounded(botao1, 0.5, sizeOftext, GREEN);
        DrawText("Fácil", botao1X + 30, botao1Y + 30, sizeOftext, WHITE);

        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 1;
            saxaDephtBoard = 0;
            *menuorboard = 1;
        }
    }

    if (CheckCollisionPointRec(mousePoint, botao2)) {

        DrawRectangleRounded(botao2, 0.5, sizeOftext, YELLOW);
        DrawText("vai travar um pouco em ...", botao1X + 30, botao2Y + 30, sizeOftext, WHITE);
        if (IsMouseButtonPressed(0)) {

            isSinglePlayer = 1;
            saxaDephtBoard = 1;
            *menuorboard = 1;
        }
    }

    if (CheckCollisionPointRec(mousePoint, botao3)) {

        DrawRectangleRounded(botao3, 0.5, sizeOftext, RED);
        DrawText("vai travar demais !!!", botao1X + 30, botao3Y + 30, sizeOftext, WHITE);
        if (IsMouseButtonPressed(0)) {
            isSinglePlayer = 1;
            saxaDephtBoard = 2;
            *menuorboard = 1;
        }
    }

    EndDrawing();
}


void tutorial(int * menuorboard) {
    Vector2 mousePoint = GetMousePosition();

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    float xOfButtons = screenWidth / 19;

    float xbotao1 = screenWidth / 13;
    float xbotao2 = screenWidth / 3;

    float larguraBorda = screenWidth / 190;
    float alturaBorda = screenHeight / 200;

    float buttonWidth = screenWidth / 5;
    float sizeOftext = screenHeight / 35;

    BeginDrawing();
    ClearBackground(BLACK); // Comando para pintar o background


    Rectangle quadro = { xOfButtons,xOfButtons,screenWidth / 2 - larguraBorda * 2,screenHeight / 2 - alturaBorda * 2 };
    Rectangle borda = { xOfButtons - larguraBorda, xOfButtons - alturaBorda,screenWidth / 2,screenHeight / 2 };

    Rectangle botao1 = { xbotao1, screenHeight / 1.5f, buttonWidth, xOfButtons };
    Rectangle botao2 = { xbotao2, screenHeight / 1.5f, buttonWidth, xOfButtons };
    Rectangle botao3 = { screenWidth / 1.35f, screenHeight / 1.5f, buttonWidth, xOfButtons };

    Rectangle txutxucaoSource = { 0,0,(float)txutxucao.width,(float)txutxucao.height };
    Rectangle txutxucaoDest = {
        screenWidth / 1.9f,
        screenHeight / 1.9f - txutxucao.height * screenHeight / 600,
        txutxucao.width * screenWidth / 950,
        txutxucao.height * screenHeight / 500
    };

    Rectangle josevalSource = { 0,0,(float)joseval.width,(float)joseval.height };

    Rectangle josevalDest = {
        screenWidth / 1.9f + txutxucao.width * screenWidth / 900,
        screenHeight / 1.9f - joseval.height * screenHeight / 150 + ((txutxucao.height * screenHeight / 500) - (joseval.height * screenHeight / 150)) / 3,
        joseval.width * screenWidth / 270,
        joseval.height * screenHeight / 150
    };



    int sizeOftextCaps = screenWidth / 40;
    int xtext = screenWidth / 19;
    int ytext = screenHeight / 9;

    int between = screenHeight / 21;


    DrawRectangleRec(borda, BROWN);
    DrawRectangleRec(quadro, DARKGREEN);

    DrawRectangleRounded(botao1, 0.5, sizeOftext, DARKGREEN);
    DrawText("Anterior", xbotao1 + screenWidth / 100, screenHeight / 1.45, sizeOftext, WHITE);

    DrawRectangleRounded(botao2, 0.5, sizeOftext, DARKGREEN);
    DrawText("Próximo", xbotao2 + screenWidth / 100, screenHeight / 1.45, sizeOftext, WHITE);

    DrawRectangleRounded(botao3, 0.5, sizeOftext, DARKGREEN);
    DrawText("Voltar", screenWidth / 1.3, screenHeight / 1.45, sizeOftext, WHITE);


    DrawTexturePro(txutxucao, txutxucaoSource, txutxucaoDest, (Vector2){ 0,0 }, 0, RAYWHITE);
    DrawTexturePro(joseval, josevalSource, josevalDest, (Vector2){ 0,0 }, 0, RAYWHITE);


    switch (quadros) {
    case 1:
        DrawText("Olá, eu sou o txutxucão ", xtext, ytext, sizeOftext, WHITE);
        DrawText("e junto com meu amigo Doutor,", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("te ensinarei como jogar xadrez", xtext, ytext + between * 2, sizeOftext, WHITE);
        break;

    case 2:
        DrawText("Bom dia, eu sou o professor da ufs", xtext, ytext, sizeOftext, WHITE);
        DrawText("e nas horas vagas passeio com minha", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("cadelinha e também jogo xadrez!", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("Irei transladar meus conhecimentos", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("enxadrísticos", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("para todos os baixinhos.", xtext, ytext + between * 5, sizeOftext, WHITE);
        break;

    case 3:
        DrawText("Vamos começar entendendo as peças...", xtext, ytext, sizeOftext, WHITE);
        DrawText("a primeira delas é o peão.", xtext, ytext + between, sizeOftextCaps, WHITE);
        DrawText("Os peões, em seus primeiros movimentos", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("podem mover-se duas casas a frente.", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("Em seus outros movimentos, podem mover-se", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("apenas uma vez e capturam outras", xtext, ytext + between * 5, sizeOftext, WHITE);
        DrawText("peças na diagonal", xtext, ytext + between * 6, sizeOftext, WHITE);
        break;

    case 4:
        DrawText("Agora as torres", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("elas podem mover-se quantas vezes quiserem", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("desde que seja para cima, para baixo", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("e para os lados.", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("Podem capturar peças a frente, atrás e aos lados.", xtext, ytext + between * 4, sizeOftext, WHITE);
        break;

    case 5:
        DrawText("Falaremos agora do cavalo", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("os cavalos movem-se de forma diferente,", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("movem-se em forma de L, ou seja,", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("andam duas casas e depois mais uma", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("em um ângulo de 90.", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("São as únicas peças que podem mover-se sobre outras.", xtext, ytext + between * 5, sizeOftext, WHITE);
        DrawText("capturam peças que estão em sua posição final", xtext, ytext + between * 6, sizeOftext, WHITE);
        break;

    case 6:
        DrawText("Vez dos bispos", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("os bispos podem mover-se quantas vezes quiser,", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("porém, só nas diagonais.", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("Os bispos começam em uma casa, seja ela clara ou escura,", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("e devem continuar sempre nesta cor.", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("Capturam as peças que estão em sua posição final,", xtext, ytext + between * 5, sizeOftext, WHITE);
        DrawText("ou seja, no final da diagonal.", xtext, ytext + between * 6, sizeOftext, WHITE);
        break;

    case 7:
        DrawText("A Rainha", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("a Rainha ou dama, é a peça mais poderosa do xadrez.", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("Move-se em qualquer linha reta, para frente, atrás,", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("aos lados ou em diagonal- contanto que não se mova", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("através de suas próprias peças.", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("A Rainha captura as peças de acordo com seu movimento.", xtext, ytext + between * 5, sizeOftext, WHITE);
        break;

    case 8:
        DrawText("O Rei", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("o Rei é a peça mais importante.", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("O Rei só pode andar uma casa em qualquer direção,", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("para frente, atrás, aos lados ou em diagonal.", xtext, ytext + between * 3, sizeOftext, WHITE);
        DrawText("O Rei nunca pode se colocar em posição de xeque- onde", xtext, ytext + between * 4, sizeOftext, WHITE);
        DrawText("pode ser capturado. Isso ocorre quando uma peça", xtext, ytext + between * 5, sizeOftext, WHITE);
        DrawText("ameaça o rei.", xtext, ytext + between * 6, sizeOftext, WHITE);
        break;

    case 9:
        DrawText("Final do jogo", xtext, ytext, sizeOftextCaps, WHITE);
        DrawText("o jogo acaba quando um dos Reis é capturado,", xtext, ytext + between, 20, WHITE);
        DrawText("daí ocorre o xeque-mate, onde o jogador que capturou", xtext, ytext + between * 2, sizeOftext, WHITE);
        DrawText("o Rei oposto, ganha.", xtext, ytext + between * 3, sizeOftext, WHITE);
        break;

    case 10:
        DrawText("Agora que vocês já aprenderam como funciona o xadrez,", xtext, ytext, sizeOftext, WHITE);
        DrawText("podem começar o jogo e se divertir,", xtext, ytext + between, sizeOftext, WHITE);
        DrawText("não é mesmo txutxucão?", xtext, ytext + between * 2, sizeOftext, WHITE);
        break;

    case 11:
        DrawText("sim, exatamente doutor", xtext, ytext, sizeOftext, WHITE);
        DrawText("boa sorte a todos os jogadores!!!!", xtext, ytext + between, sizeOftext, WHITE);
        break;
    }

    if (CheckCollisionPointRec(mousePoint, botao1)) {

        DrawRectangleRounded(botao1, 0.5, sizeOftext, RED);
        DrawText("Anterior", xbotao1 + screenWidth / 100, screenHeight / 1.45, sizeOftext, WHITE);

        if (IsMouseButtonPressed(0)) {
            if (quadros != 1) {
                quadros--;
            }
        }
    };

    if (CheckCollisionPointRec(mousePoint, botao2)) {

        DrawRectangleRounded(botao2, 0.5, sizeOftext, GREEN);
        DrawText("Próximo", xbotao2 + screenWidth / 100, screenHeight / 1.45, sizeOftext, WHITE);

        if (IsMouseButtonPressed(0)) {
            if (quadros != 11)
                quadros++;
        }
    };

    if (CheckCollisionPointRec(mousePoint, botao3)) {

        DrawRectangleRounded(botao3, 0.5, sizeOftext, GREEN);
        DrawText("Voltar", screenWidth / 1.30, screenHeight / 1.45, sizeOftext, WHITE);

        if (IsMouseButtonPressed(0)) {
            *menuorboard = 0;
        }
    };


    EndDrawing(); // Fechando a parte do desenho

}


Board BoardInit(int screenWidth, int screenHeight) {
    Board board;
    memset(&board, 0, sizeof(Board));

    board.squareBlackColor = PINK;
    board.squareWhiteColor = WHITE;

    board.pieceSpriteSheet = LoadTexture("assets/chess_pieces.png");
    board.pieceSpriteSize.x = board.pieceSpriteSheet.width / 6.f;
    board.pieceSpriteSize.y = board.pieceSpriteSheet.height / 2.f,
  

    board.state.fullmoves = 1;

    BoardResize(&board, screenWidth, screenHeight);
    _BoardLoadFEN(&board);

    return board;
}

void _BoardInit(Board* board) {
    board->backButtonClicked = false;

    memset(&board->movingPiece, 0, sizeof board->movingPiece);
    memset(&board->move, 0, sizeof board->movingPiece);

    BoardLoadFEN(board, BOARD_FEN_DEFAULT);
}

bool BoardLoadFEN(Board* board, const char fen[BOARD_FEN_LENGTH]) {
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

bool _BoardLoadFEN(Board* board) {
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

bool BoardSaveFEN(Board* board) {

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


bool BoardMakeMove(Board* board, int from, int to, bool updateWhoMoves) {
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
        board->state.waitPromotion = PieceFile(to) + 1;

        if (PieceHasColor(board->squares[from], PIECE_WHITE))
            board->state.waitPromotion |= 0b1000;

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

bool BoardKingInCheck(Board board, int kingColor) {
    for (int square = 0; square < 64; square++)
        if (board.move.pseudoLegalMoves[square]
            && PieceHasType(board.squares[square], PIECE_KING)
            && PieceHasColor(board.squares[square], kingColor))
            return true;

    return false;
}

bool BoardKingInMate(Board board, int kingColor) {
    if (BoardKingInCheck(board, kingColor) && board.move.count == 0)
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

    /* Resize the board if screen size has changed */
    if (IsWindowResized())
        BoardResize(board, GetScreenWidth(), GetScreenHeight());

    if (board->state.waitPromotion) {
        updatePromotionMenu(board);

        if (!board->state.waitPromotion) {
            if (board->state.whoMoves == PIECE_BLACK) {
                saxa_move Move = saxamove(*board, saxaDephtBoard, PIECE_BLACK);
                BoardMakeMove(board, Move.from, Move.to, true);
            }
        }
    }
    else if (BoardKingInMate(*board, board->state.whoMoves)) {
        // Essa parte está sendo implementada em drawMateWindow
    }
    else if (board->backButtonClicked) {
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
                    && PieceHasColor(board->squares[square], board->state.whoMoves)) {
                    board->movingPiece.dragging = true;
                    board->movingPiece.selecting = false;

                    clickTime = 0;
                }

                if (!board->movingPiece.dragging
                    && !PieceHasType(board->squares[square], PIECE_NONE)
                    && PieceHasColor(board->squares[square], board->state.whoMoves))
                    board->movingPiece.position = square;
            }
            else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
                if ((board->movingPiece.dragging
                    || (board->movingPiece.selecting
                        && board->movingPiece.position != square))
                    && clicked) {
                    board->movingPiece.dragging = false;
                    board->movingPiece.selecting = false;

                    BoardMakeMove(board, board->movingPiece.position, square, true);
                }

                if (clicked && GetTime() - clickTime <= CLICK_TIME
                    && !board->movingPiece.selecting
                    && !PieceHasType(board->squares[square], PIECE_NONE)
                    && PieceHasColor(board->squares[square], board->state.whoMoves)) {
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

    if (isSinglePlayer && !board->state.waitPromotion) {
        if (board->state.whoMoves == PIECE_BLACK) {
            saxa_move Move = saxamove(*board, saxaDephtBoard, PIECE_BLACK);
            BoardMakeMove(board, Move.from, Move.to, true);
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

    Color color, opositeColor;

    for (int square = 0; square < 64; square++) {
        squarePosition.x = board->drawPosition.x + squarePosition.width
            * PieceFile(square);
        squarePosition.y = board->drawPosition.y + squarePosition.height
            * PieceRank(square);

        if ((PieceRank(square) + PieceFile(square)) % 2) {


            if (selectionB == 0) {
                color = board->squareBlackColor;
            }
            else {
                color = colors[selectionB];
            }

            opositeColor = board->squareWhiteColor;

        }
        else {
            color = WHITE;
            opositeColor = color;
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

    if (!board->state.waitPromotion)
        drawDraggingPiece(*board, squarePosition);

    drawMateWindow(board, menu);
    drawPromotionMenu(*board);
    backButton(board, menu);
}

static void generateMoves(Board * board, bool onlyLegalMoves) {

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

static void generateKingMoves(Board * board, int square, bool legalMove) {

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
        if (BoardKingInCheck(*board, PieceGetColor(board->squares[square])))
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
                board->move.pseudoLegalMoves[targetSquare] = true;
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

static void generateSlightMoves(Board * board, int square, bool legalMove) {
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
                board->move.pseudoLegalMoves[targetSquare] = true;
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

static void generateKnightMoves(Board * board, int square, bool legalMove) {
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
            board->move.pseudoLegalMoves[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            board->move.list[square][targetSquare] = MOVE_NORMAL;
            board->move.count++;
        }
    }
}

static void generatePawnMoves(Board * board, int square, bool legalMove) {
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
            board->move.pseudoLegalMoves[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            if (moves == 1) {
                if (targetSquare > 7 && targetSquare < 56)
                    board->move.list[square][targetSquare] = MOVE_NORMAL;
                else
                    board->move.list[square][targetSquare] = MOVE_PAWN_PROMOTE;
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
            board->move.pseudoLegalMoves[targetSquare] = true;
        }
        else if (isValidMove(*board, square, targetSquare)) {
            if (targetSquare == board->state.enPassantSquare) {
                board->move.list[square][targetSquare] = MOVE_PAWN_EN_PASSANT;
                board->move.count++;
            }

            if (!PieceHasType(board->squares[targetSquare], PIECE_NONE)
                && !PieceHasColor(board->squares[targetSquare], PieceGetColor(board->squares[square]))) {
                if (targetSquare > 7 && targetSquare < 56)
                    board->move.list[square][targetSquare] = MOVE_NORMAL;
                else
                    board->move.list[square][targetSquare] = MOVE_PAWN_PROMOTE;

                board->move.count++;
            }
        }
    }
}

static void updateTurn(Board* board, bool updateWhoMove) {
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

static bool isValidMove(Board board, int from, int to) {
    Board copiedBoard;

    memcpy(&copiedBoard, &board, sizeof(board));
    copiedBoard.move.list[from][to] = MOVE_NORMAL;

    BoardMakeMove(&copiedBoard, from, to, false);
    generateMoves(&copiedBoard, false);

    for (int square = 0; square < 64; square++)
        if (copiedBoard.move.pseudoLegalMoves[square]
            && PieceHasType(copiedBoard.squares[square], PIECE_KING)
            && PieceHasColor(copiedBoard.squares[square], copiedBoard.state.whoMoves))
            return false;

    return true;
}

static void updatePromotionMenu(Board * board) {
    static int promotionSelected = 5;

    const int rank = (board->state.waitPromotion & 0b1000) == 0 ? 7 : 0;
    const int file = (board->state.waitPromotion - 1) & 0b0111;

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

    if (!board->state.waitPromotion)
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
            board->squares[PieceSquare(rank, file)] = promotionSelected + PIECE_KING;

            if (board->state.waitPromotion & 0b1000)
                board->squares[PieceSquare(rank, file)] += PIECE_WHITE;
            else
                board->squares[PieceSquare(rank, file)] += PIECE_BLACK;

            updateTurn(board, false);

            promotionSelected = 5;
            board->state.waitPromotion = 0;
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
                            BoardSaveFEN(board);
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
        && !board.state.waitPromotion)
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

    if (PieceHasType(board.squares[square], PIECE_NONE))
        return;

    /* Draw a red square if the king is in check */
    if (PieceHasType(board.squares[square], PIECE_KING)
        && BoardKingInCheck(board, PieceGetColor(board.squares[square])))
        DrawRectangleRec(drawPosition, RED);

    /* Draw the piece image if isn't dragging any piece or if is dragging
     * doesn't the piece is this square.
     */
    if ((!board.movingPiece.dragging && !board.movingPiece.selecting)
        || board.movingPiece.position != square) {
        spritePosition.x = spritePosition.width * (PieceGetType(board.squares[square]) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(board.squares[square], PIECE_BLACK);

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

        spritePosition.x = spritePosition.width * (PieceGetType(board.squares[square]) - 1);
        spritePosition.y = spritePosition.height * PieceHasColor(board.squares[square], PIECE_BLACK);

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
        || !board.move.list[board.movingPiece.position][square])
        return;

    drawPosition.x += drawPosition.width / 2.f;
    drawPosition.y += drawPosition.height / 2.f;

    if (PieceHasType(board.squares[square], PIECE_NONE))
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

    spritePosition.x = spritePosition.width * (PieceGetType(board.squares[board.movingPiece.position]) - 1);
    spritePosition.y = spritePosition.height * PieceHasColor(board.squares[board.movingPiece.position], PIECE_BLACK);

    drawPosition.x = GetMouseX() - board.squareLength / 2.f;
    drawPosition.y = GetMouseY() - board.squareLength / 2.f;

    DrawTexturePro(board.pieceSpriteSheet, spritePosition, drawPosition, (Vector2){ 0, 0 }, 0, WHITE);
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
        board->pieceSpriteSize.y * (board->state.whoMoves != PIECE_BLACK),

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

    const char* wonText = board->state.whoMoves == PIECE_WHITE ? "PRETO VENCEU!" : "BRANCO VENCEU!";
    const int wonTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(wonText, wonFontSize) / 2.f;
    const int wonTextPosY = (spritePosition.y + spritePosition.height) * 1.004f;

    const char* btnText = "Voltar ao menu";
    const int btnTextPosX = windowRectangle.x + windowRectangle.width / 2 - MeasureText(btnText, btnFontSize) / 2.f;
    const int btnTextPosY = btnRectangle.y + btnRectangle.height / 2.f - btnFontSize / 2.f;

    btnRectangle.width = MeasureText(btnText, btnFontSize) * 1.5f;
    btnRectangle.x += windowRectangle.width / 2.f - btnRectangle.width / 2.f;

    if (!BoardKingInMate(*board, board->state.whoMoves))
        return;

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
        board.pieceSpriteSize.y * ((board.state.waitPromotion & 0b1000) == 0),

        board.pieceSpriteSize.x,
        board.pieceSpriteSize.y,
    };

    Rectangle spritePosition = {
        menuRectangle.x + menuRectangle.width / 18.f,
        menuRectangle.y + menuRectangle.height / 7.f,

        menuRectangle.width / 4.5f,
        menuRectangle.height / 1.5f,
    };

    if (!board.state.waitPromotion)
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


