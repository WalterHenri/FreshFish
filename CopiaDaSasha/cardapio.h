bool checkTextColision(Vector2 origin, Vector2 position, Font font, char* text, int fontSize, int spacing) {

    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    if ((origin.x >= position.x && origin.x <= position.x + size.x)
        && (origin.y >= position.y - size.y / 2 && origin.y <= position.y + size.y / 2)) {
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
    DrawTexture(background, 0, 0, WHITE);

    Vector2 positionTitle;
    const int spacing = 2;
    const int fontSizeTitle = screenHeight / 6;

    positionTitle.x = screenwidth / 2 - MeasureTextEx(titleFont, "Fresh Fish", fontSizeTitle, 2).x / 2;
    positionTitle.y = screenHeight / 3;

    DrawTextEx(titleFont, "Fresh Fish", positionTitle, fontSizeTitle, spacing, WHITE);

    const int fontSizeOption = screenHeight / 20;
    Vector2 positionJogar;
    positionJogar.x = screenwidth / 2 - MeasureTextEx(optionsFont, "JOGAR", fontSizeOption, 2).x / 2;
    positionJogar.y = screenHeight / 2;

    DrawTextEx(optionsFont, "JOGAR", positionJogar, fontSizeOption, spacing, WHITE);

    Vector2 positionOpcoes;
    positionOpcoes.x = screenwidth / 2 - MeasureTextEx(optionsFont, "OPCOES", fontSizeOption, 2).x / 2;
    positionOpcoes.y = positionJogar.y + (MeasureTextEx(optionsFont, "JOGAR", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "OPCOES", positionOpcoes, fontSizeOption, spacing, WHITE);

    Vector2 positionSair;
    positionSair.x = screenwidth / 2 - MeasureTextEx(optionsFont, "SAIR", fontSizeOption, 2).x / 2;
    positionSair.y = positionOpcoes.y + (MeasureTextEx(optionsFont, "OPCOES", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "SAIR", positionSair, fontSizeOption, spacing, WHITE);



    if (checkTextColision(mousePoint, positionJogar, optionsFont, "JOGAR", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "JOGAR", positionJogar, fontSizeOption, spacing, YELLOW);

        Vector2 newPosition = positionJogar;
        const double scale = 0.15;
        newPosition.x -= scale * playHoverText.width + spacing;
        DrawTextureEx(playHoverText, newPosition, 0, scale, WHITE);

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

    char cores[11][50] = { "rosinha", "azul","roxinho","verdinho","dourado",
                            "vermelho","vinho","esgoto","beeeje","escuro","rosa choque"
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

    Vector2 positionSetup;
    positionSetup.x = screenwidth / 2 - MeasureTextEx(optionsFont, "MONTAR TABULEIRO", fontSizeOption, 2).x / 2;
    positionSetup.y = positionSair.y + (MeasureTextEx(optionsFont, "RETROCERDER", fontSizeOption, 2).y);

    DrawTextEx(optionsFont, "MONTAR TABULEIRO", positionSetup, fontSizeOption, spacing, WHITE);


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

    if (checkTextColision(mousePoint, positionSetup, optionsFont, "MONTAR TABULEIRO", fontSizeOption, spacing)) {
        DrawTextEx(optionsFont, "MONTAR TABULEIRO", positionSetup, fontSizeOption, spacing, YELLOW);
        if (IsMouseButtonPressed(0)) {
            *menuorboard = 7;
        }
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
    positionOpcoes.y = positionJogar.y + (MeasureTextEx(optionsFont, "FACIL", fontSizeOption, 2).y) * 2;

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