#pragma once


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

Sound sndCheck;
Sound sndCapture;
Sound sndMove;
Music musMenu;

void menuInit() {
    //upload das imagens usadas

    logo = LoadTexture("./assets/FreshFishLogo.png");
    pecas = LoadTexture("./assets/chess_pieces.png");
    background = LoadTexture("./assets/initialScreen.png");
    background2 = LoadTexture("./assets/menuScreen.png");
    titleFont = LoadFontEx("./Fonts/HipotesiS_Tittle.ttf", 256, 0, 0);
    optionsFont = LoadFontEx("./Fonts/LemonMilk_Content.otf", 256, 0, 0);
    playHoverText = LoadTexture("./assets/chess-game.png");
    optionHoverText = LoadTexture("./assets/gear.png");
    bgLeft = LoadTexture("./assets/bg.png");


    sndCapture = LoadSound("./sound effects/capture.mp3");
    sndMove = LoadSound("./sound effects/move.mp3");
 
    musMenu = LoadMusicStream("./sound effects/musicaDeMenu.mp3");
    sndCheck = LoadSound("./sound effects/check.mp3");
}
