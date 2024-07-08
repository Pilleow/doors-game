/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions
#include "constants.h"

#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_MP3

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = GAMEPLAY;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
Music bgMusic[bgMusicCount];
Sound sfxShoot[sfxShootCount];
Sound sfxHit[sfxHitCount];
Sound sfxDead[sfxDeadCount];
static GameScreen transToScreen = UNKNOWN;
RenderTexture2D screenRenderTexture;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //---------------------------------------------------------

    InitWindow(1, 1, "Door Game");
    int monitor = GetCurrentMonitor();
    finalWidth = GetMonitorWidth(monitor);
    finalHeight = GetMonitorHeight(monitor);
    CloseWindow();

    InitWindow(finalWidth, finalHeight, "Door Game");
    ToggleFullscreen();

    screenRenderTexture = LoadRenderTexture(screenWidth, screenHeight);

    srand(time(NULL));      // Initialize random module
    InitAudioDevice();      // Initialize audio device
    SetMasterVolume(masterVolume);

    // Load sfx here individually
    for (int i = 0; i < sfxShootCount; ++i) {
        char filename[32] = "resources/sfx/shoot/shoot%d.wav";
        sprintf(filename, filename, i);
        sfxShoot[i] = LoadSound(filename);
        SetSoundVolume(sfxShoot[i], sfxShootVolume);
    }
    for (int i = 0; i < sfxDeadCount; ++i) {
        char filename[32] = "resources/sfx/dead/dead%d.wav";
        sprintf(filename, filename, i);
        sfxDead[i] = LoadSound(filename);
        SetSoundVolume(sfxDead[i], sfxDeadVolume);
    }
    for (int i = 0; i < sfxHitCount; ++i) {
        char filename[32] = "resources/sfx/hit/hit%d.wav";
        sprintf(filename, filename, i);
        sfxHit[i] = LoadSound(filename);
        SetSoundVolume(sfxHit[i], sfxHitVolume);
    }

    // Load bg music files here individually
    bgMusic[0] = LoadMusicStream("resources/bgm/gnosisHardware.mp3");
    for (int i = 0; i < bgMusicCount; ++i) SetMusicVolume(bgMusic[i], bgMusicVolume);

    PlayMusicStream(bgMusic[0]);

    // Load global data (assets that must be available in all screens, i.e. font)
//    font = LoadFont("resources/font/BebasNeue-Regular.ttf");
//    fxCoin = LoadSound("resources/coin.wav");


    // Setup and init first screen
    currentScreen = GAMEPLAY;
    InitGameplayScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen) {
        case GAMEPLAY:
            UnloadGameplayScreen();
            break;
        default:
            break;
    }

    // Unload global data loaded
    for (int i = 0; i < bgMusicCount; ++i) UnloadMusicStream(bgMusic[i]);
    for (int i = 0; i < sfxShootCount; ++i) UnloadSound(sfxShoot[i]);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen) {
    // Unload current screen
    switch (currentScreen) {
        case GAMEPLAY:
            UnloadGameplayScreen();
            break;
        default:
            break;
    }

    // Init next screen
    switch (screen) {
        case GAMEPLAY:
            InitGameplayScreen();
            break;
        default:
            break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen) {
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void) {
    if (!transFadeOut) {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f) {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen) {
                case GAMEPLAY:
                    UnloadGameplayScreen();
                    break;
                default:
                    break;
            }

            // Load next screen
            switch (transToScreen) {
                case GAMEPLAY:
                    InitGameplayScreen();
                    break;
                default:
                    break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    } else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f) {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    // Update
    //----------------------------------------------------------------------------------
    for (int i = 0; i < bgMusicCount; ++i) UpdateMusicStream(bgMusic[i]); // NOTE: Music keeps playing between screens

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    if (!onTransition) {
        switch (currentScreen) {
            case GAMEPLAY: {
                UpdateGameplayScreen();

//                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            }
                break;
            default:
                break;
        }
    } else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(screenRenderTexture);
    ClearBackground(RAYWHITE);
    switch (currentScreen) {
        case GAMEPLAY:
            DrawGameplayScreen();
            break;
        default:
            break;
    }
    if (onTransition) DrawTransition();
    EndTextureMode();

    BeginDrawing();
    DrawTexturePro(
            screenRenderTexture.texture,
            (Rectangle) {0, 0, (float) (screenRenderTexture.texture.width),
                         (float) (-screenRenderTexture.texture.height)},
            (Rectangle) {0, 0, (float) (finalWidth), (float) (finalHeight)},
            (Vector2) {0, 0},
            0,
            WHITE);
    EndDrawing();
    //----------------------------------------------------------------------------------
}
