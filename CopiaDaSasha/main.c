#pragma warning(disable : 4996)

#include "raylib.h"

#include "Board.h"

#include <stdio.h>

#define SCREEN_TITLE  "FreshFish"


int main(void) {

    int menuorboard = 0; //variavel para saber onde ta o menu

    Board board;


    InitWindow(0, 0, SCREEN_TITLE);
    //SetWindowState(FLAG_FULLSCREEN_MODE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    /* Set the initial mouse position one pixel at bottom left of
     * the center of the window. This prevent some bugs when start
     * drawing the square that's the mouse is in.
     */

    SetMousePosition(screenWidth / 2 + 1, screenHeight / 2 + 1);

    initHashKeys();
    board = BoardInit(screenWidth, screenHeight);

    
    InitAudioDevice();
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
        case 3:
            gamemode(&board, &menuorboard);
            break;
        case 4:
            gameDificult(&menuorboard);
            break;
        case 7:
            setupPosition(&board, &menuorboard);
            break;
        }
    }
    BoardUnload(&board);
    //Unloads dos arquivos que foram feitos uploads

    CloseWindow();

    return 0;
}



