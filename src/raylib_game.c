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
#include "screens.h"
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
Sound sfxDoorOpen[sfxDoorOpenCount];
Sound sfxShoot[sfxShootCount];
Sound sfxDash[sfxDashCount];
Sound sfxDead[sfxDeadCount];
Sound sfxHit[sfxHitCount];
RenderTexture2D screenRenderTexture;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Required variables to manage screen transitions (fade-in, fade-out)
static Music startMusic;
static Music bgMusic[bgMusicCount];
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static float bgmPreviousTime = -1;
static int currentMusicIndex = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect
static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
static void UpdateDrawFrame(void);          // Update and draw one frame


// A function to generate a random permutation of arr[]
void shuffleArray(Music arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Music temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void loadSfxIntoArray(Sound sfxArray[], char filename[], int totalCount, float initVolume) {
    for (int i = 0; i < totalCount; ++i) {
        char fn[64];
        sprintf(fn, filename, i);
        sfxArray[i] = LoadSound(fn);
        SetSoundVolume(sfxArray[i], initVolume);
    }
}

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
    loadSfxIntoArray(sfxShoot, "resources/sfx/shoot/shoot%d.wav", sfxShootCount, sfxShootVolume);
    loadSfxIntoArray(sfxDead, "resources/sfx/dead/dead%d.wav", sfxDeadCount, sfxDeadVolume);
    loadSfxIntoArray(sfxHit, "resources/sfx/hit/hit%d.wav", sfxHitCount, sfxHitVolume);
    loadSfxIntoArray(sfxDoorOpen, "resources/sfx/doorOpen/doorOpen%d.wav", sfxDoorOpenCount, sfxDoorOpenVolume);
    loadSfxIntoArray(sfxDash, "resources/sfx/dash/dash%d.wav", sfxDashCount, sfxDashVolume);

    // Load bg music files here individually
    startMusic = LoadMusicStream("resources/bgm/gnosisHardware.mp3");
    bgMusic[0] = LoadMusicStream("resources/bgm/comeTrue.mp3");
    bgMusic[1] = LoadMusicStream("resources/bgm/akihabara.mp3");
    bgMusic[2] = LoadMusicStream("resources/bgm/outHereAtTheAtmosphere.mp3");
    bgMusic[3] = LoadMusicStream("resources/bgm/passportsOne.mp3");
    bgMusic[4] = LoadMusicStream("resources/bgm/chaos.mp3");
    bgMusic[5] = LoadMusicStream("resources/bgm/chicago.mp3");
    bgMusic[6] = LoadMusicStream("resources/bgm/nothingStopsDetroit.mp3");
    bgMusic[7] = LoadMusicStream("resources/bgm/passportsTwo.mp3");
    SetMusicVolume(startMusic, bgMusicVolume);
    for (int i = 0; i < bgMusicCount; ++i) SetMusicVolume(bgMusic[i], bgMusicVolume);
    shuffleArray(bgMusic, bgMusicCount);

    PlayMusicStream(startMusic);

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

    // Init next screen
    switch (screen) {
        case GAMEPLAY:
            InitGameplayScreen();
            break;
        case LEVELEDITOR:
            InitLevelEditorScreen();
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

//            // Unload current screen
//            switch (transFromScreen) {
//                case GAMEPLAY:
//                    UnloadGameplayScreen();
//                    break;
//                default:
//                    break;
//            }

            // Load next screen
            switch (transToScreen) {
                case GAMEPLAY:
                    InitGameplayScreen();
                    break;
                case LEVELEDITOR:
                    InitLevelEditorScreen();
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
    if (currentMusicIndex == -1) {
        UpdateMusicStream(startMusic);
        if (bgmPreviousTime > GetMusicTimePlayed(startMusic)) {
            PauseMusicStream(startMusic);
            StopMusicStream(startMusic);
            currentMusicIndex = 0;
            PlayMusicStream(bgMusic[currentMusicIndex]);
            bgmPreviousTime = -1;
        }
//        else if (GetMusicTimePlayed(startMusic) < 5)
//        SeekMusicStream(startMusic, GetMusicTimeLength(startMusic) - 3);
        bgmPreviousTime = GetMusicTimePlayed(startMusic);
    } else {
        UpdateMusicStream(bgMusic[currentMusicIndex]);
        if (bgmPreviousTime > GetMusicTimePlayed(bgMusic[currentMusicIndex])) {
            PauseMusicStream(bgMusic[currentMusicIndex]);
            StopMusicStream(bgMusic[currentMusicIndex]);
            currentMusicIndex++;
            bgmPreviousTime = -1;
            if (currentMusicIndex >= bgMusicCount) {
                currentMusicIndex = 0;
                shuffleArray(bgMusic, bgMusicCount);
            }
            PlayMusicStream(bgMusic[currentMusicIndex]);
        }
//        else if (GetMusicTimePlayed(bgMusic[currentMusicIndex]) < 5)
//        SeekMusicStream(bgMusic[currentMusicIndex], GetMusicTimeLength(bgMusic[currentMusicIndex]) - 3);
        bgmPreviousTime = GetMusicTimePlayed(bgMusic[currentMusicIndex]);
    }

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    if (!onTransition) {
        switch (currentScreen) {
            case GAMEPLAY:
                UpdateGameplayScreen();
                if (GotoLevelEditorScreen()) TransitionToScreen(LEVELEDITOR);
                break;
            case LEVELEDITOR:
                UpdateLevelEditorScreen();
                if (GotoGameplayScreen()) TransitionToScreen(GAMEPLAY);
            default:
                break;
        }
    } else UpdateTransition();    // Update transition (fade-in, fade-out)

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(screenRenderTexture);
    ClearBackground(BLACK);
    switch (currentScreen) {
        case GAMEPLAY:
            DrawGameplayScreen(false);
            break;
        case LEVELEDITOR:
            DrawLevelEditorScreen();
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
}
