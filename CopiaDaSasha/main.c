#pragma warning(disable : 4996)
#include "raylib.h"
#include "Board.h"
#include <stdio.h>

#define SCREEN_TITLE  "Xuxa"


int main(void) {

    int menuorboard = 0; //variavel para saber onde ta o menu

    Board board;

    InitWindow(0, 0, SCREEN_TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    /* Set the initial mouse position one pixel at bottom left of
     * the center of the window. This prevent some bugs when start
     * drawing the square that's the mouse is in.
     */

    SetMousePosition(screenWidth / 2 + 1, screenHeight / 2 + 1);

    board = BoardInit(screenWidth, screenHeight);

    menuInit();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        switch (menuorboard) {

        case 0:
            menu(&menuorboard);
            break;

        case 1:
            /* Update section:
            *   update the game logic, doesn't use any draw function here
            */
            BoardUpdate(&board);

            /* Draw section:
             *  this is the correct place to use draw functions, but this time logic
             *  update should be avoided
             */
            BeginDrawing();
            ClearBackground(BLACK);

            DrawFPS(0, 0);


            BoardDraw(&board, &menuorboard);

            EndDrawing();
            break;
        case 2:
            tutorial(&menuorboard);
            break;
        case 3:
            gamemode(&menuorboard);
            break;
        case 4:
            gameDificult(&menuorboard);
            break;
        case 5:
            login(&menuorboard);
            break;
        case 6:
            GameBoard(&menuorboard);
            break;

        }
    }
    BoardUnload(&board);
    //Unloads dos arquivos que foram feitos uploads

    CloseWindow();

    return 0;
}


